#ifndef _HIGHLIGHT_NEWS_HANDLE_H__
#define _HIGHLIGHT_NEWS_HANDLE_H__

#include "Storage.h"

#include "Common/Base/flags.h"
#include "Common/Base/stringUtil.h"
#include "Robot/Public/Proto/TrioBrainServing.h"

#include "time.h"
#include <string>
using namespace std;

namespace robot {

#define BILLION 1000000000L     // for timer usage

class TrioBrainServingHandler : public TrioBrainServingIf{

public:
	TrioBrainServingHandler(){}
	~TrioBrainServingHandler(){}
	bool Init(ESConfig* e, Swap* s);
	virtual void TrioBrainProcess(robot::TrioDataBundle&respone, const robot::TrioDataBundle& request);

private:
        void GetObject(Storage** storage, pthread_rwlock_t** lock);
        void UpdateStorage();

private:
        ESConfig* esc;
        Swap* swp;

};//end class

}//end namespace
#endif

