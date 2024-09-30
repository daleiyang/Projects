//from common
#include "Common/Base/flags.h"
#include "Common/Base/Internal/versionPrint.h"
#include "Common/Base/globalInit.h"
#include "Common/ThirdParty/thrift/include/concurrency/PosixThreadFactory.h"
#include "Common/ThirdParty/thrift/include/concurrency/ThreadManager.h"
#include "Common/ThirdParty/thrift/include/protocol/TBinaryProtocol.h"
#include "Common/ThirdParty/thrift/include/server/TNonblockingServer.h"
#include "Common/ThirdParty/thrift/include/Thrift.h"
//zookeeper
#include "Common/ThirdParty/zk_wrapper/NSServer.h"

//from c++
#include <iostream>
#include <string>

//from others
#include "Storage.h"
#include "HighlightNewsServingHandle.h"

//shared objects
robot::Swap* swp;

using apache::thrift::TProcessor;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::concurrency::ThreadManager;
using apache::thrift::concurrency::PosixThreadFactory;
using apache::thrift::server::TNonblockingServer;

//thrift
DEFINE_int32(port, 15726, "serving port");
DEFINE_int32(threads, 24, "serving threads");
//es
DEFINE_string(esHost, "10.0.1.22:15723", "ES host");
DEFINE_string(esIndex, "highlight_news", "ES index");
//zookeeper
DEFINE_string(zoo_server, "172.16.0.244:5181", "zookeeper server host:port");
DEFINE_string(zoo_node, "/Content/projects/hot_news", "register a node on zk");
DEFINE_bool(zoo_enable, true, "enable zookeeper register or not");

int main(int argc, char** argv) {
    //parse parameters
    InitFlagsAndVersion(&argc, &argv);

    //init log
    global_init::InitGlog(argv[0]);

    //load ES config
    robot::ESConfig* esc = new robot::ESConfig();
    esc->esHost = FLAGS_esHost;
    esc->esIndex = FLAGS_esIndex;

    //init storge object
    robot::Storage* s = new robot::Storage(esc); 
    if(!s->Init()){
        VLOG(0) << "Init Storage failed, exit whole process.";
        exit(0);
    }

    swp = new robot::Swap();
    swp->flag = 1; //f == 1 means A is active
    swp->A = s;
    swp->updating = 0; //init, 0 means no updating is in processing.

    //init thrift
    boost::shared_ptr<robot::TrioBrainServingHandler> handler(new robot::TrioBrainServingHandler());
    if (handler.get() == NULL) {return -1;}
    if (!handler->Init(esc, swp)) {return -1;}
    boost::shared_ptr<TProcessor> processor(new robot::TrioBrainServingProcessor(handler));
    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());
    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(FLAGS_threads);
    boost::shared_ptr<PosixThreadFactory> thread_factory(new PosixThreadFactory());
    thread_manager->threadFactory(thread_factory);
    thread_manager->start();
    TNonblockingServer server(processor, protocol_factory, FLAGS_port, thread_manager);
    VLOG(1) << "Thrift init successsfully.";

    //zookeeper
    using ::util::NSServer;
    NSServer ns_server;
    if(FLAGS_zoo_enable){
        uint16 port = FLAGS_port;
        map<string, pair<string, string> > param;
        ns_server.ZooInit(FLAGS_zoo_server, 510);
        CHECK(ns_server.RegisterOnZK(FLAGS_zoo_node, port, param));
        VLOG(0)<<"zookeeper init successfully.";
    }
    //start server
    server.serve();

    return 0;
}

