#ifndef _HIGHLIGHT_NEWS_HANDLE_H__
#define _HIGHLIGHT_NEWS_HANDLE_H__

#include "Storage.h"

#include "Common/Base/flags.h"
#include "Common/Base/stringUtil.h"
#include "Robot/Public/Proto/TrioBrainServing.h"
#include "Common/Util/Log/TrioLog.h"
#include "Common/Util/ThriftClientWrapper/thrift.h"

//boost
#include "Common/ThirdParty/boost/algorithm/string/trim.hpp"

#include <string>
#include <vector>
using namespace std;
using namespace triolog;

namespace robot {

class TrioBrainServingHandler : public TrioBrainServingIf{

public:
    TrioBrainServingHandler(){}
    ~TrioBrainServingHandler(){}
    bool Init(Swap* s, TmallShoppingContentPlugin* t);
    virtual void TrioBrainProcess(robot::TrioDataBundle&respone, const robot::TrioDataBundle& request);

private:
    void GetObject(Storage** storage, pthread_rwlock_t** lock);
    void UpdateStorage();

private:
    int maxl = 1024;
    TrioLog& logInstance = TrioLog::GetInstance();
    Swap* swp;
    TmallShoppingContentPlugin* tscp;

};//end class

}//end namespace
#endif

