#ifndef STORAGE_H_
#define STORAGE_H_

//from Common library
#include "Common/Base/stringUtil.h"
#include "Common/Base/flags.h"
#include "Common/Base/time.h"
#include "Common/Util/Config/Config.h"
#include "Common/Util/HttpClient/curlWrapper.h"
#include "Common/ThirdParty/jsoncpp-0.5.0/include/json/json.h"
#include "Common/Util/Trie/jumpTrie.h"

//from C library
#include "pthread.h"
#include "ctype.h"

//boost
#include "Common/ThirdParty/boost/algorithm/string.hpp"

//from C++ library
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

namespace robot {

struct Logging{
public:
    string traceId;
    string userId;
    string deviceId;
public:
    void log(int level, string mess){
        VLOG(level) << "traceId = " << traceId << '\t' 
                    << "userId = " << userId << '\t'
                    << "deviceId = " << deviceId << '\t'
                    << "message = " << mess;
    }
};

struct ESConfig{
public:
   string esHost, esIndex;
};

struct Detail{
    string type;
    string rule;
    string url;
    string detail;
    long starttime;
    long endtime;
    int deep;
    bool operator < (const Detail& r) const{
        return  deep > r.deep;
    }
};

struct Trie{
    int sigma_size;
    vector<vector<int>* > ch;     
    map<int, vector<Detail*>* > val;
    int sz; 

    void init(int size){
        sz = 1;
        sigma_size = size;
        //root node
        vector<int>* v = new vector<int>(sigma_size, 0);
        ch.push_back(v);
    }

    void insert(vector<int>& s, string& type, string& rule, string& url, string& detail, long st, long et){
        int u = 0, n = s.size();
        for(int i = 0; i < n; i++){
            if(!(*ch[u])[s[i]]){
                vector<int>* v = new vector<int>(sigma_size, 0);
                ch.push_back(v);
                (*ch[u])[s[i]] = sz++; 
            }
            u = (*ch[u])[s[i]];
        }
        if(!val.count(u)) val[u] = new vector<Detail*>();
        Detail* d = new Detail();
        d->type = type;
        d->rule = rule;
        d->url = url;
        d->detail = detail;
        d->starttime = st;
        d->endtime = et;
        d->deep = 0;
        val[u]->push_back(d);
    }

    void longestMatch(vector<int>& s, vector<Detail*>& details){
        int n = s.size(), longest = 0;
        vector<vector<Detail*>* > result;
        //start position
        for(int i = 0; i < n; i++){
            int u = 0, deep = 0;
            for(int j = i; j < n; j++){
                if(!(*ch[u])[s[j]]) {
                    if(!u) break; 
                    else continue;
                }
                else{
                    u = (*ch[u])[s[j]];
                    deep++;
                    if(val.count(u)){
                        vector<Detail*>* v = val[u];
                        for(int x = 0; x < (int)v->size(); x++){
                            v->at(x)->deep = deep;
                        }
                        if(deep > longest){
                            longest = deep;
                            result.clear();
                            result.push_back(v);
                        }
                        else if(deep == longest){
                            result.push_back(v);
                        }
                    }
                }
            }
        } 
        for(int i = 0; i < (int)result.size(); i++){
            for(int j = 0; j < (int)result[i]->size(); j++){
                details.push_back(result[i]->at(j));
            }
        }
    }
 
    void allMatch(vector<int>& s, vector<Detail*>& details){
        int n = s.size();
        vector<vector<Detail*>* > result;
        //start position
        for(int i = 0; i < n; i++){
            int u = 0, deep = 0;
            for(int j = i; j < n; j++){
                if(!(*ch[u])[s[j]]) {
                    if(!u) break; 
                    else continue;
                }
                else{
                    u = (*ch[u])[s[j]];
                    deep++;
                    if(val.count(u)){
                        vector<Detail*>* v = val[u];
                        for(int x = 0; x < (int)v->size(); x++){
                            v->at(x)->deep = deep;
                        }
                        result.push_back(v);
                    }
                }
            }
        } 
        for(int i = 0; i < (int)result.size(); i++){
            for(int j = 0; j < (int)result[i]->size(); j++){
                details.push_back(result[i]->at(j));
            }
        }
    }

    void log(){
        //for trie node
        for(int i = 0; i < sz; i++){ 
            VLOG(4) << "No." << i;
            string t = "";
            for(int j = 0; j < sigma_size; j++){
                t += to_string((*ch[i])[j]) + " "; 
            } 
            VLOG(4) << t;
        } 

        //for all node value
        map<int, vector<Detail*>* >::iterator it;     
        for(it = val.begin(); it != val.end(); it++){
            VLOG(4) << "Node num = " << it->first;
            vector<Detail*>* t = it->second;
            for(int i = 0; i < (int)t->size(); i++){
                Detail* t2 = t->at(i);
                VLOG(4) << "Content is = " << t2->detail;
                VLOG(4) << "Starttime is = " << t2->starttime;
                VLOG(4) << "Endtime is = " << t2->endtime;
            }
        }
    }

    void clean(){
        //delete detail  
        map<int, vector<Detail*>* >::iterator it;     
        for(it = val.begin(); it != val.end(); it++){
            vector<Detail*>* t = it->second;
            for(int i = 0; i < (int)t->size(); i++) delete (*t)[i];   
            delete t;
        }
        //delete ch array
        for(int i = 0; i < ch.size(); i++) delete ch[i]; 
    }
};

struct Rule{
    vector<int> keywords;
    string type;
    string rule;
    string url;
    string content;
    long st;
    long et;
};

class Storage{
private:
    ESConfig* es;
    int maxl = 2048;

    trie::JumpTrie* acAutoKeyword;

    //for trie
    vector<string> words;
    set<string> wordsSet;
    map<string, int> wordsIdx;    
    vector<Rule*> rules;
    Trie trie;

public:
    Storage(ESConfig* es);
    ~Storage();
    bool Init();
    void ReleaseScroll(string& host, string& scrollId);
    bool ValidateString(string& input);
    long ParseTime(string& time);
    void Log();
    bool Search(string& query, int cnt, string& result, Logging& log);
    void Clean();

private:
    bool LoadFromES();
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

