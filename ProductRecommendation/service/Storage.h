#ifndef STORAGE_H_
#define STORAGE_H_

//from Common library
#include "Common/Base/stringUtil.h"
#include "Common/Base/flags.h"
#include "Common/Base/time.h"
#include "Common/Util/File/simpleLineReader.h"
#include "Common/Util/Config/Config.h"
#include "Common/Util/HttpClient/curlWrapper.h"
#include "Common/Util/Encode/encodeUtil.h"
#include "Common/Util/Log/TrioLog.h"
#include "Common/Util/Trie/jumpTrie.h"
#include "Common/ThirdParty/openssl/include/md5.h"
#include "Common/ThirdParty/jsoncpp-0.5.0/include/json/json.h"
#include "Common/ThirdParty/hiredis/hiredis.h"
#include "frame_semantic_parser_new/QueryPreProcesserWrapper.h"
#include "frame_semantic_parser_new/Utils.h"

//from C library
#include "pthread.h"
#include "time.h"

//boost
#include "Common/ThirdParty/boost/algorithm/string/trim.hpp"
#include "Common/ThirdParty/boost/algorithm/string.hpp"

//from C++ library
#include <string>
#include <map>
#include <set>
#include <vector>
#include <pthread.h>

#include "tfCWrapper.h"
#include "TmallShoppingContentPlugin.h"

using namespace std;
using namespace triolog;

namespace robot {

#define BILLION 1000000000L

class Parameter{
public:
    string type;
    string deviceId;
    string query;
    vector<string> wareNames;
    vector<string> imageUrls;
    vector<string> prices;
    vector<string> skuIds;
    string result;
    bool success;
public:
    Parameter(string type, string query): type(type), query(query){
        success = false;
    } 
    Parameter(string type, string deviceId, string query): type(type), deviceId(deviceId), query(query){
        success = false;
    } 
    ~Parameter(){}
};

class Storage{
public:
    Storage();
    ~Storage();
    bool Init(TmallShoppingContentPlugin* t);
    bool Search(string& traceId, string& userId, string& deviceId, string& query, string& result);
    void* Worker(Parameter* p);
    void Clean();
    bool CheckJDAPI(string& query, string& result);
    bool CheckTmallAPI(string& query, string& deviceId,  string& result);
    bool CheckBrandAPI(string& query, string& result);

private:
    TFCWrapper* wrapper;
    map<string, int>* wordId;
    vector<string>* category;
    map<string, string> whiteList;
    TmallShoppingContentPlugin* tscp;
    TrioLog& logInstance = TrioLog::GetInstance();

    //constant
    string imageUrlBase = "http://img11.360buyimg.com/n1/";
    string itemUrlBase = "https://item.jd.com/";
    string baseIntentUrlPre = "openapp.jdmobile://virtual?params={\\\"category\\\":\\\"jump\\\",\\\"des\\\":\\\"productDetail\\\",\\\"skuId\\\":\\\"";
    string baseIntentUrlPost = "\\\",\\\"sourceType\\\":\\\"unknown\\\",\\\"sourceValue\\\":\\\"unknown\\\",\\\"M_sourceFrom\\\":\\\"sxtop\\\",\\\"msf_type\\\""
      ":\\\"click\\\",\\\"m_param\\\":{\\\"m_source\\\":\\\"0\\\",\\\"event_series\\\":{},\\\"jda\\\":\\\"122270672.15127185830131661088761.1512718583.1512718583.1512718583.1\\\"" 
      ",\\\"usc\\\":\\\"baidu-pinzhuan\\\",\\\"ucp\\\":\\\"t_288551095_baidupinzhuan\\\",\\\"umd\\\":\\\"cpc\\\",\\\"utr\\\":\\\"36e78a1acce545f59c3461c890d009a9_0_4d3505762a844dbabcc13d4dedf87db4\\\""
      ",\\\"jdv\\\":\\\"122270672%7Cbaidu-pinzhuan%7Ct_288551095_baidupinzhuan%7Ccpc%7C36e78a1acce545f59c3461c890d009a9_0_4d3505762a844dbabcc13d4dedf87db4%7C1512718583038\\\""
      ",\\\"ref\\\":\\\"https%3A%2F%2Fitem.m.jd.com%2Fproduct%2F2397497.html\\\",\\\"psn\\\":\\\"15127185830131661088761|1\\\",\\\"psq\\\":4,\\\"unpl\\\":\\\"\\\",\\\"pc_source\\\":\\\"\\\",\\\"mba_muid\\\""
      ":\\\"15127185830131661088761\\\",\\\"mba_sid\\\":\\\"15127185830524599070494097937\\\",\\\"mt_xid\\\":\\\"\\\",\\\"mt_subsite\\\":\\\"\\\"},\\\"SE\\\":{\\\"mt_subsite\\\":\\\"\\\",\\\"__jdv\\\""
      ":\\\"122270672%7Cbaidu-pinzhuan%7Ct_288551095_baidupinzhuan%7Ccpc%7C36e78a1acce545f59c3461c890d009a9_0_4d3505762a844dbabcc13d4dedf87db4%7C1512718583038\\\",\\\"unpl\\\""
      ":\\\"\\\",\\\"__jda\\\":\\\"122270672.15127185830131661088761.1512718583.1512718583.1512718583.1\\\"}}";

    //Redis
    static redisContext *redis;
    string host;
    int port;
    string password;
    int maxTryTimes;
    string expiresTime;

private:
    bool JDAPI(string& query, vector<string>& wareNames, vector<string>& imageUrls, vector<string>& prices, vector<string>& skuIds);
    bool InitCnn();
    bool Inference(string& input, vector<float>& prob);
    int FindCharSize(const char* s, int idx);
    bool TmallBrandApi(string& query, string& result);
    bool TmallProductApi(string& query, string& deviceId, string& result );
    bool GenerateFinalResult(string& qtype, string& type_result, string& result );
};

class Launcher{
public:
    Storage* st;
    Parameter* pa;
public:
    Launcher(){}
    Launcher(Storage* st, Parameter* pa): st(st), pa(pa) {}
    void *launch(){ return st->Worker(pa);}
};

struct Swap{
public:
    int flag = 1; //1 means A is active, 0 means B is active
    pthread_mutex_t flagMutex = PTHREAD_MUTEX_INITIALIZER;

    Storage* A = NULL;
    pthread_rwlock_t rwlockA = PTHREAD_RWLOCK_INITIALIZER;

    Storage* B = NULL;
    pthread_rwlock_t rwlockB = PTHREAD_RWLOCK_INITIALIZER;

    int updating = 0;// 1 means update in process, 0 means no update happening
    pthread_mutex_t updatingMutex = PTHREAD_MUTEX_INITIALIZER;

    Swap(){}
};

} //end namespace
#endif // STORAGE_H_

