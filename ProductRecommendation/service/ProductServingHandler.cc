#include "ProductServingHandler.h";

namespace robot {

bool TrioBrainServingHandler::Init(Swap* s, TmallShoppingContentPlugin* t){ 
    swp = s;
    tscp = t;
    return true; 
}

void TrioBrainServingHandler::TrioBrainProcess(robot::TrioDataBundle& response, 
                                               const robot::TrioDataBundle& request){
    int64 usec_a = common::GetTimeInUsec();

    bool CheckJDAPI = false;
    bool CheckTmallAPI = false;
    bool CheckBrandAPI = false;

    if(request.__isset.params){
        robot::ControlParams control_params;
        control_params = request.params;
        map<string, string> custom;
        custom = control_params.custom;

        VLOG(0) << "update_storage " << custom["update_storage"];
        VLOG(0) << "check_jd_api " << custom["check_jd_api"];
        VLOG(0) << "check_tmall_api " << custom["check_tmall_api"];
        VLOG(0) << "check_brand_api " << custom["check_brand_api"];

        string update = custom["update_storage"];
        if(update == "true"){
            VLOG(0) << "Update process start.";
            UpdateStorage();
            VLOG(0) << "Update process end.";
            return;
        }
        if(custom["check_jd_api"] == "true") CheckJDAPI = true;
        if(custom["check_tmall_api"] == "true") CheckTmallAPI = true;
        if(custom["check_brand_api"] == "true") CheckBrandAPI = true;
    }

    std::string query = "";
    if(!request.__isset.query){
        VLOG(0) << "request.query is not set. Cancle this request.";
        return;
    }
    query = request.query;
    if(strlen(query.c_str()) > maxl){
        VLOG(0) << "Query too long, ignore it.";
        return;
    }

    string traceId = "content_default_trace_id";
    if(request.__isset.params && request.params.__isset.trace_id){
        traceId = request.params.trace_id;             
    }
    VLOG(4) << "Trace Id = " << traceId;
 
    string userId = "content_default_user_id";
    if(request.__isset.user && request.user.__isset.uid){
        userId = request.user.uid; 
    }
    VLOG(4) << "user Id = " << userId;

    string deviceId = "content_default_device_id";
    if(request.__isset.terminal && request.terminal.__isset.mac){
        deviceId = request.terminal.mac;
    }
    VLOG(4) << "device Id = " << deviceId;

    //Get current Storage object;
    Storage* storage = NULL;
    pthread_rwlock_t* lock = NULL;
    GetObject(&storage, &lock);

    string result = "{\"type\": -1}";

    if(CheckJDAPI){ storage->CheckJDAPI(query, result); }
    else if(CheckTmallAPI){ storage->CheckTmallAPI(query, userId, result); }
    else if(CheckBrandAPI){ storage->CheckBrandAPI(query, result); }
    else { storage->Search(traceId, userId, deviceId, query, result); }
 
    pthread_rwlock_unlock(lock); //Unlock storage read lock;

    robot::RespApiCustom apiCustom;
    apiCustom.__set_key("Classification");  
    apiCustom.__set_value_json(result);
    vector<robot::RespApiCustom> apiCustoms;
    apiCustoms.push_back(apiCustom);

    robot::RespApiData apiData;
    apiData.__set_custom_info(apiCustoms);

    vector<robot::RespApiData> apiDatas;
    apiDatas.push_back(apiData);

    robot::RespApiParams apiParams;
    apiParams.__set_data(apiDatas);

    response.__set_api_resp(apiParams);

    int64 usec_b = common::GetTimeInUsec();
    stringstream stream;
    stream << ((double)(usec_b - usec_a))/((double)1000);
    string timeCost = stream.str();
    logInstance.LogInfo("request", traceId, userId, deviceId, timeCost, query, "content_default_response");
    logInstance.LogInfo("response", traceId, userId, deviceId, timeCost, query, FromThriftToUtf8DebugString(&response));

    return;
}

void TrioBrainServingHandler::UpdateStorage(){
    pthread_mutex_lock(&(swp->updatingMutex));//identify update process is working or not 
    if(swp->updating == 1){
        pthread_mutex_unlock(&(swp->updatingMutex));
        string errorMessage = "Updating process is working now, cancel this update request.";
        VLOG(0) << errorMessage; 
        return;
    }
    else if(swp->updating == 0){
        swp->updating = 1;
        pthread_mutex_unlock(&(swp->updatingMutex));
    }

    Storage* s = new Storage();
    if(!s->Init(tscp)){
        VLOG(0) << "Init Storage failed. Cancel this update request.";
        return;
    }

    int freeTarget = 0;//1 means free A, 0 means free B
    pthread_mutex_lock(&(swp->flagMutex));//Control swap happening 
    if(swp->flag == 1){
        swp->flag = 0;
        swp->B = s;
        freeTarget = 1; //need to free A
    }
    else if(swp->flag == 0){
        swp->flag = 1;
        swp->A = s;
        freeTarget = 0; //need to free B
    }
    pthread_mutex_unlock(&(swp->flagMutex));

    if(freeTarget == 1){//free A
        //We acqurie write lock here to make sure all read process finished.
        pthread_rwlock_wrlock(&(swp->rwlockA));
        swp->A->Clean();
        pthread_rwlock_unlock(&(swp->rwlockA));
        delete(swp->A);
        swp->A = NULL; }
    else if(freeTarget == 0){//free B
        //We acqurie write lock here to make sure all read process finished.
        pthread_rwlock_wrlock(&(swp->rwlockB));
        swp->B->Clean();
        pthread_rwlock_unlock(&(swp->rwlockB));
        delete(swp->B);
        swp->B = NULL;
    }

    //mark update process finished
    pthread_mutex_lock(&(swp->updatingMutex));
    swp->updating = 0;
    pthread_mutex_unlock(&(swp->updatingMutex));

    return; 
}

void TrioBrainServingHandler::GetObject(Storage** storage, pthread_rwlock_t** lock){
    pthread_mutex_lock(&(swp->flagMutex));//Block swap happening 
    if(swp->flag == 1){
       //When we free A in swap process: (1)Make sure no more read lock comes in. (2)Acqrire a write lock.
       //We are secure to free A after all read locks are unlocked.
       pthread_rwlock_rdlock(&(swp->rwlockA));
       *storage = swp->A;
       *lock = &(swp->rwlockA); //Need to unlock it outside;
       pthread_mutex_unlock(&(swp->flagMutex));//Allow swap happening
    }
    else{
       //When we free B in swap process: (1)Make sure no more read lock comes in. (2)Acqrire a write lock.
       //We are secure to free B after all read locks are unlocked.
       pthread_rwlock_rdlock(&(swp->rwlockB));
       *storage = swp->B;
       *lock = &(swp->rwlockB); //Need to unlock it outside;
       pthread_mutex_unlock(&(swp->flagMutex));//Allow swap happending
    }
    return;
}

}

