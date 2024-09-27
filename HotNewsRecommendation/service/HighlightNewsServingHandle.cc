#include "HighlightNewsServingHandle.h";

namespace robot {

bool TrioBrainServingHandler::Init(ESConfig* e, Swap* s){ 
    esc = e;
    swp = s;
    return true; 
}

void TrioBrainServingHandler::TrioBrainProcess(robot::TrioDataBundle& response, 
                                               const robot::TrioDataBundle& request){

    unsigned long long diff;
    struct timespec s1, e1, s2, e2;
    double used;
    clock_gettime(CLOCK_MONOTONIC, &s1);

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

    Logging log;  
    log.traceId = traceId;
    log.userId = userId;
    log.deviceId = deviceId;

    int cnt = 1;
 
    if(request.__isset.params){
        robot::ControlParams control_params;
        control_params = request.params;
        map<string, string> custom;
        custom = control_params.custom;
         
        //for update logic only
        string update = custom["update_storage"];
        if(update == "true"){
            VLOG(0) << "Update process start.";
            UpdateStorage();
            VLOG(0) << "Update process end.";
            return;
        }

        try{
            cnt = std::stoi(custom["show_size"]);
        }
        catch(std::invalid_argument const &e){
            log.log(0, "Bad input show_size=" + custom["show_size"]);
        }
        catch(std::out_of_range const &e){
            log.log(0, "Integer overflow show_size=" + custom["show_size"]);
        }
        log.log(1, "Result size=" + to_string(cnt));
    }

    std::string query = "";
    if(request.__isset.query) query = request.query;
    log.log(0, "Query = " + query);

    //Get current Storage object;
    Storage* storage = NULL;
    pthread_rwlock_t* lock = NULL;
    GetObject(&storage, &lock);
 
    string result="";    
    clock_gettime(CLOCK_MONOTONIC, &s2);
    storage->Search(query, cnt, result, log);
    clock_gettime(CLOCK_MONOTONIC, &e2);
    diff = BILLION * (e2.tv_sec - s2.tv_sec) + e2.tv_nsec - s2.tv_nsec;
    used = (double)diff / BILLION;
    log.log(0, "Storage.Search finished in " + to_string(used));

    pthread_rwlock_unlock(lock); //Unlock storage read lock;

    robot::RespApiCustom apiCustom;
    apiCustom.__set_key("highlightNews");  
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

    clock_gettime(CLOCK_MONOTONIC, &e1);
    diff = BILLION * (e1.tv_sec - s1.tv_sec) + e1.tv_nsec - s1.tv_nsec;
    used = (double)diff / BILLION;
    log.log(1, "TrioBrainProcess finihed in " + to_string(used));

    return;
}

void TrioBrainServingHandler::UpdateStorage(){
    VLOG(0) << "Ask updatingMutex"; 
    pthread_mutex_lock(&(swp->updatingMutex));//identify update process is working or not 
    VLOG(0) << "Got updatingMutex"; 
    if(swp->updating == 1){
        pthread_mutex_unlock(&(swp->updatingMutex));
        VLOG(0) << "Release updatingMutex"; 
        string errorMessage = "Updating process is working now, cancel this update request";
        VLOG(0) << errorMessage; 
        return;
    }
    else if(swp->updating == 0){
        swp->updating = 1;
        pthread_mutex_unlock(&(swp->updatingMutex));
        VLOG(0) << "Release updatingMutex"; 
    }

    Storage* s = new Storage(esc);
    if(!s->Init()){
        string errorMessage = "Init Storage failed, abort update process.";
        VLOG(0) << errorMessage; 
    }
    else{
        int freeTarget = 0;//1 means free A, 0 means free B
        VLOG(0) << "Ask flagMutex"; 
        pthread_mutex_lock(&(swp->flagMutex));//Control swap happening 
        VLOG(0) << "Got flagMutex"; 
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
        VLOG(0) << "Release flagMutex"; 

        if(freeTarget == 1){//free A
            //We acqurie write lock here to make sure all read process finished.
            VLOG(0) << "Ask rwlockA write lock"; 
            pthread_rwlock_wrlock(&(swp->rwlockA));
            VLOG(0) << "Got rwlockA write lock"; 
            VLOG(0) << "Start A->Clean()";
            swp->A->Clean();
            VLOG(0) << "End A->Clean()";
            VLOG(0) << "Ask rwlockA write unlock"; 
            pthread_rwlock_unlock(&(swp->rwlockA));
            VLOG(0) << "Got rwlockA write unlock"; 
            VLOG(0) << "Start delete A";
            delete(swp->A);
            swp->A = NULL; 
            VLOG(0) << "End delete A";
            }
        else if(freeTarget == 0){//free B
            //We acqurie write lock here to make sure all read process finished.
            VLOG(0) << "Ask rwlockB write lock"; 
            pthread_rwlock_wrlock(&(swp->rwlockB));
            VLOG(0) << "Got rwlockB write lock"; 
            VLOG(0) << "Start B->Clean()";
            swp->B->Clean();
            VLOG(0) << "End B->Clean()";
            VLOG(0) << "Ask rwlockB write unlock"; 
            pthread_rwlock_unlock(&(swp->rwlockB));
            VLOG(0) << "Got rwlockB write unlock"; 
            VLOG(0) << "Start delete B";
            delete(swp->B);
            swp->B = NULL;
            VLOG(0) << "End delete B";
        }
    }

    //mark update process finished
    VLOG(0) << "Ask updatingMutex"; 
    pthread_mutex_lock(&(swp->updatingMutex));
    VLOG(0) << "Got updatingMutex"; 
    swp->updating = 0;
    pthread_mutex_unlock(&(swp->updatingMutex));
    VLOG(0) << "Release updatingMutex"; 

    //logging
    VLOG(0) << "swp->flag = " << swp->flag; 
    VLOG(0) << "swp->updating = " << swp->updating; 
    
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

