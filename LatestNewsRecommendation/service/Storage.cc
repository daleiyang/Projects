#include "Storage.h"

namespace robot {

Storage::Storage(ESConfig* esc){ 
    es = esc;
    acAutoKeyword = new trie::JumpTrie();
    acAutoKeyword->Build();
}

Storage::~Storage(){ }

bool Storage::Init(){

    if(!LoadFromES()) return false;

    Log();
 
    //Build Trie
    VLOG(0) << "Start build Trie.";
    trie.init(words.size());
    for(int i = 0; i < (int)rules.size(); i++){
        Rule* r = rules[i];
        string tr = "";

        //for logging
        bool b = true;
        for(int j = 0; j <(int)r->keywords.size(); j++){
            if(b){
                tr += words[r->keywords[j]];
                b = false;
            }
            else{
                tr += " & " + words[r->keywords[j]];
            }
        }
        VLOG(2) << "Rule = " << tr;

        trie.insert(r->keywords, r->type, r->rule, r->url, r->content, r->st, r->et);
        delete r;
    }
    VLOG(0) << "End build Trie.";

    //Check trie logic
    VLOG(0) << "Start Trie Log.";
    trie.log();
    VLOG(0) << "End Trie Log.";

    //Build acAutoKeyword 
    VLOG(0) << "Start build acAuto.";
    map<string, int>::iterator it;
    for(it = wordsIdx.begin(); it != wordsIdx.end(); it++)
        acAutoKeyword->AddString(it->first);
    acAutoKeyword->Build(); 
    VLOG(0) << "End build acAuto.";
 
    return true;
}

bool Storage::LoadFromES(){
    string host = es->esHost;
    string index = es->esIndex;
    string url, postData, output, errmsg;
    string esScrollLife = "10m";
    string esBatchSize = "9999";
    int64_t start, end;

    //init scroll get
    url = "http://" + host + "/" + index + "/_search?scroll="+ esScrollLife  +"&pretty"; 
    postData = "{";
    postData += "\"size\": "+ esBatchSize  +",";
    postData += "\"query\":{\"match_all\":{}}";
    postData += "}";

    VLOG(4) << "url: " << url;
    VLOG(4) << "PostData: " << postData;

    start = GetTimeInMs();
    if(CurlWrapper::GetPostRespondData(url, postData, &output, &errmsg)){
        VLOG(0) << "Curl Get Customized data failed";
        return false;
    }
    end = GetTimeInMs() - start;
    VLOG(4) << "Netword IO time for Get Customized data:" << end/1000.0 << " seconds.";
    VLOG(4) << output;
    VLOG(4) << errmsg;

    Json::Reader reader(Json::Features::strictMode()); 
    Json::Value root;
    if(!reader.parse(output, root)){ VLOG(0) << "parse json request failed"; return false;}

    if(root["_scroll_id"].isNull()){ VLOG(0) << "scorll id is empty"; return false; }
    string scrollId = root["_scroll_id"].asString();
    VLOG(4) << scrollId;

    if(root["hits"].isNull()){ VLOG(0) << "no hit information"; return false; }  
    if(root["hits"]["hits"].isNull()){ VLOG(0) << "no hit detail information"; return false; }

    int hitsSize  = root["hits"]["hits"].size(); 
    VLOG(4) << "hit size is:" << hitsSize;
    if(!hitsSize) { ReleaseScroll(host, scrollId); return true; }

    for(size_t i = 0; i < hitsSize; i++){
        if(root["hits"]["hits"][i]["_id"].isNull() ||
            root["hits"]["hits"][i]["_source"].isNull() ||
            root["hits"]["hits"][i]["_source"]["details"].isNull() ||
            root["hits"]["hits"][i]["_source"]["starttime"].isNull() ||
            root["hits"]["hits"][i]["_source"]["endtime"].isNull() ||
            root["hits"]["hits"][i]["_source"]["normalized"].isNull() ||
            root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].isNull()) continue;

        //parse keywords, starttime, endtime, detail
        Json::Value res;
        res["result"] = root["hits"]["hits"][i]["_source"]["details"];
        Json::FastWriter fast_writer;
        string content = fast_writer.write(res);
        VLOG(4) << "details:" << endl << content;

        string url = "";
        if(!root["hits"]["hits"][i]["_source"]["details"]["url"].isNull()){
            url = root["hits"]["hits"][i]["_source"]["details"]["url"].asString();
        }

        string starttime = root["hits"]["hits"][i]["_source"]["starttime"].asString();
        if(!ValidateString(starttime)) { VLOG(0) << "starttime:" << starttime << " is invalided"; continue; }
        VLOG(4) << "starttime:" << starttime;

        string endtime = root["hits"]["hits"][i]["_source"]["endtime"].asString();
        if(!ValidateString(endtime)) { VLOG(0) << "endtime:" << endtime << " is invalided"; continue; }
        VLOG(4) << "endtime:" << endtime;

        time_t nt; time(&nt);
        long st = ParseTime(starttime);
        long et = ParseTime(endtime); 
        if(!(st <= (long)nt && et >= (long)nt)) {
            VLOG(2) << "Expired content, starttime = " + starttime + " endtime = " + endtime;
            VLOG(2) << "Content = " + content; 
            continue;
        }

        if(!root["hits"]["hits"][i]["_source"]["normalized"].isNull() &&
           !root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].isNull()){

            int ts = 0;
            if(!root["hits"]["hits"][i]["_source"]["normalized"]["flag"].isNull()){
                ts = root["hits"]["hits"][i]["_source"]["normalized"]["flag"].size();
            }
 
            int size  = root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].size(); 
            if(!size) { VLOG(0) << "normalized size is:" << size << " is invalided"; }
            else{
                for(size_t j = 0; j < size; j++){
                    string temp = root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"][j].asString();  
                    if(!ValidateString(temp)) { VLOG(0) << "Input rule:" << temp << "is invalided"; continue; }
                    boost::algorithm::to_lower(temp);
                    vector<string> strs;
                    boost::split(strs, temp, boost::is_any_of("&"));
                    vector<int> keywords;
                    for(int k = 0; k < (int)strs.size(); k++){
                        if(!ValidateString(strs[k])) { VLOG(0) << "Input keyword:" << strs[k] << "is invalided"; continue; }
                        if(!wordsSet.count(strs[k])){
                            wordsSet.insert(strs[k]);
                            words.push_back(strs[k]); 
                            wordsIdx[strs[k]] = words.size() - 1;
                        }
                        int idx = wordsIdx[strs[k]];
                        keywords.push_back(idx);
                    }
                    if(keywords.size()){
                        sort(keywords.begin(), keywords.end());
                        vector<int>::iterator it; 
                        it = unique(keywords.begin(), keywords.end());
                        keywords.resize(distance(keywords.begin(), it));
                        Rule* r = new Rule();
                        r->keywords = keywords;
                        if(ts != size) { r->type = ""; }
                        else {
                            string t = root["hits"]["hits"][i]["_source"]["normalized"]["flag"][j].asString();
                            if(!ValidateString(t)) { 
                                VLOG(0) << "Input type:" << t << "is invalided";
                                r->type = ""; 
                            }
                            else{
                                boost::algorithm::to_lower(t);
                                r->type = t;
                            }
                        }
                        r->rule = temp;
                        r->url = url;
                        r->content = content;
                        r->st = ParseTime(starttime);
                        r->et = ParseTime(endtime);
                        rules.push_back(r);
                    }
                }
            }
        }
    }      
 
    //more scroll get until we get nothing in one round
    while(hitsSize){
        url = "http://" + host + "/_search/scroll?pretty"; 
        postData = "{";
        postData += "\"scroll\": \""+ esScrollLife +"\",";
        postData += "\"scroll_id\":\""+ scrollId +"\"";
        postData += "}";

        VLOG(4) << "url: " << url;
        VLOG(4) << "PostData: " << postData;

        string output, errmsg;
        int64_t start = GetTimeInMs();
        if(CurlWrapper::GetPostRespondData(url, postData, &output, &errmsg)){
                VLOG(0) << "Curl Get Customized data failed";
                return false;
        }
        int64_t end = GetTimeInMs() - start;
        VLOG(4) << "Netword IO time for Get Customized data:" << end/1000.0 << " seconds.";
        VLOG(4) << output;
        VLOG(4) << errmsg;

        Json::Reader reader(Json::Features::strictMode()); 
        Json::Value root;
        if(!reader.parse(output, root)){ VLOG(0) << "parse json request failed"; return false;	}

        if(root["_scroll_id"].isNull()){ VLOG(0) << "scorll id is empty"; return false; }
        scrollId = root["_scroll_id"].asString();
        VLOG(4) << scrollId;

        if(root["hits"].isNull()){ VLOG(0) << "no hit information"; return false; }  
        if(root["hits"]["hits"].isNull()){ VLOG(0) << "no hit detail information"; return false; }

        hitsSize = root["hits"]["hits"].size(); 
        VLOG(4) << "hit size is:" << hitsSize;
        if(!hitsSize) { ReleaseScroll(host, scrollId); return true; }

        //more scroll get until we get nothing in one round
        for(size_t i = 0; i < hitsSize; i++){
            if(root["hits"]["hits"][i]["_id"].isNull() ||
                root["hits"]["hits"][i]["_source"].isNull() ||
                root["hits"]["hits"][i]["_source"]["details"].isNull() ||
                root["hits"]["hits"][i]["_source"]["starttime"].isNull() ||
                root["hits"]["hits"][i]["_source"]["endtime"].isNull() ||
                root["hits"]["hits"][i]["_source"]["normalized"].isNull() ||
                root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].isNull()) continue;

            //parse keywords, starttime, endtime, detail
            Json::Value res;
            res["result"] = root["hits"]["hits"][i]["_source"]["details"];
            Json::FastWriter fast_writer;
            string content = fast_writer.write(res);
            VLOG(4) << "details:" << endl << content;

            string url = "";
            if(!root["hits"]["hits"][i]["_source"]["details"]["url"].isNull()){
                url = root["hits"]["hits"][i]["_source"]["details"]["url"].asString();
            }

            string starttime = root["hits"]["hits"][i]["_source"]["starttime"].asString();
            if(!ValidateString(starttime)) { VLOG(0) << "starttime:" << starttime << "is invalided"; continue; }
            VLOG(4) << "starttime:" << starttime;

            string endtime = root["hits"]["hits"][i]["_source"]["endtime"].asString();
            if(!ValidateString(endtime)) { VLOG(0) << "endtime:" << endtime << "is invalided"; continue; }
            VLOG(4) << "endtime:" << endtime;

            time_t nt; time(&nt);
            long st = ParseTime(starttime);
            long et = ParseTime(endtime); 
            if(!(st <= (long)nt && et >= (long)nt)) {
                VLOG(2) << "Expired content, starttime = " + starttime + " endtime = " + endtime;
                VLOG(2) << "Content = " + content; 
                continue;
            }

            if(!root["hits"]["hits"][i]["_source"]["normalized"].isNull() &&
               !root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].isNull()){

                int ts = 0;
                if(!root["hits"]["hits"][i]["_source"]["normalized"]["flag"].isNull()){
                    ts = root["hits"]["hits"][i]["_source"]["normalized"]["flag"].size();
                }

                int size  = root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"].size(); 
                if(!size) { VLOG(0) << "normalized size is:" << size << " is invalided"; }
                else{
                    for(size_t j = 0; j < size; j++){
                        string temp = root["hits"]["hits"][i]["_source"]["normalized"]["name_lists"][j].asString();  
                        if(!ValidateString(temp)) { VLOG(0) << "Input rule:" << temp << "is invalided"; continue; }
                        boost::algorithm::to_lower(temp);
                        vector<string> strs;
                        boost::split(strs, temp, boost::is_any_of("&"));
                        vector<int> keywords;
                        for(int k = 0; k < (int)strs.size(); k++){
                            if(!ValidateString(strs[k])) { VLOG(0) << "Input keyword:" << strs[k] << "is invalided"; continue; }
                            if(!wordsSet.count(strs[k])){
                                wordsSet.insert(strs[k]);
                                words.push_back(strs[k]); 
                                wordsIdx[strs[k]] = words.size() - 1;
                            }
                            int idx = wordsIdx[strs[k]];
                            keywords.push_back(idx);
                        }
                        if(keywords.size()){
                            sort(keywords.begin(), keywords.end());
                            vector<int>::iterator it; 
                            it = unique(keywords.begin(), keywords.end());
                            keywords.resize(distance(keywords.begin(), it));
                            Rule* r = new Rule();
                            r->keywords = keywords;
                            if(size != ts) { r->type = ""; }
                            else {
                                string t = root["hits"]["hits"][i]["_source"]["normalized"]["flag"][j].asString();
                                if(!ValidateString(t)) { 
                                    VLOG(0) << "Input type:" << t << "is invalided";
                                    r->type = ""; 
                                }
                                else{
                                    boost::algorithm::to_lower(t);
                                    r->type = t;
                                }
                            }
                            r->rule = temp;
                            r->url = url;
                            r->content = content;
                            r->st = ParseTime(starttime);
                            r->et = ParseTime(endtime);
                            rules.push_back(r);
                        }
                    }
                }
            }
        }      
    }
    return true;
}

bool Storage::Search(string& query, int cnt, string& result, Logging& log){
    //setup default return value
    Json::Value root;
    root["hasResults"] = "false";
    Json::FastWriter fast_writer;
    result = fast_writer.write(root);

    boost::algorithm::trim(query);
    if(!strlen(query.c_str())){ 
        log.log(0, "Query is empty.");
        return false;
    } 

    if(strlen(query.c_str()) > maxl){ 
        log.log(0, "Query is too long, ignore this request.  Query = " + query);
        return false;
    } 

    boost::algorithm::to_lower(query);

    //search keyword
    vector<const string*> res;
    vector<int> index;
    acAutoKeyword->LongestMatchWithIndex(query, &res, &index);

    //detect english fragment
    vector<string> res_t;
    for(int i = 0; i < (int)res.size(); i++){
        string keyword = *res[i];
        bool flag = true;
        for(int j = 0; j < strlen(keyword.c_str()); j++){
            if(!isalpha(keyword.c_str()[j])){
                flag = false; 
                break;
            }
        } 
        if(flag){
            bool f = false;
            //check left
            if(index[i] > 0){
                log.log(0, "left = " + query.c_str()[index[i] - 1]);
                if(isalpha(query.c_str()[index[i] - 1])) f = true;
            }       
            //check right
            if((index[i] + strlen(keyword.c_str())) < strlen(query.c_str())){
                log.log(0, "right = " + query.c_str()[index[i] + strlen(keyword.c_str())]);
                if(isalpha(query.c_str()[index[i] + strlen(keyword.c_str())])) f = true;
            }
            if(!f) res_t.push_back(keyword);
        }
        else res_t.push_back(keyword);
    }

    //remove duplicate
    vector<int> idx;
    for(int i = 0; i < (int)res_t.size(); i++){
        idx.push_back(wordsIdx[res_t[i]]);
    } 
    sort(idx.begin(), idx.end());
    vector<int>::iterator it; 
    it = unique(idx.begin(), idx.end());
    idx.resize(distance(idx.begin(), it));
 
    if(idx.size()){
        string tr = "";
        for(int i = 0; i < (int)idx.size(); i++){
            tr = tr + "(" + to_string(idx[i]) + ")" + words[idx[i]];
        }
        log.log(1, "Pattern in query = " + tr);
    }
    else log.log(1, "No pattern found in query.");

    //check matching rule
    vector<Detail*> t1;
    //trie.longestMatch(idx, details);
    trie.allMatch(idx, t1);
    if(!t1.size()){
        log.log(1, "No matching rule was found.");
        return false;
    }
 
    vector<Detail> details;
    for(int i = 0; i < (int)t1.size(); i++){
        details.push_back(*t1[i]);
    }
    sort(details.begin(), details.end());

    bool ret = false;
    string rule = "";
    string type = "";
    Json::Value results;
    time_t nt; time(&nt);
    int num = 0;

    set<string> duplicate;
 
    for(int j = 0; (j < (int)details.size()) && (num < cnt); j++){
        if(details[j].starttime <= (long)nt && details[j].endtime >= (long)nt){
            Json::Reader reader(Json::Features::strictMode()); 
            Json::Value content;
            if(!reader.parse(details[j].detail, content)){
                log.log(1, "Parse json string failed.");
                log.log(1, "Rule = " + details[j].rule);
                log.log(1, "Content = " + details[j].detail);
            }
            else{
                if(content["result"].isNull() ||
                   content["result"]["cardtag"].isNull() ||
                   content["result"]["desc"].isNull() ||
                   content["result"]["id"].isNull() ||
                   content["result"]["name"].isNull() ||
                   content["result"]["pic"].isNull() ||
                   content["result"]["url"].isNull() ||
                   content["result"]["sub_contents"].isNull())
                {
                    log.log(0, "content format is wrong");
                    log.log(0, content.asString());
                    continue;
                }

                string img = "";
                string title = "";
                string url = "";
 
                bool f = false;
                const Json::Value temp = content["result"]["sub_contents"];
                for(int x = 0; x < (int)temp.size(); x++){
                    if(temp[x]["news_lists"].isNull()){
                        log.log(0, "news_lists is not exists.");
                        log.log(0, content.asString());
                        continue;
                    }
                    const Json::Value temp2 = temp[x]["news_lists"];
                    for(int y = 0; y < (int)temp2.size(); y++){
                        const Json::Value temp3 = temp2[y];
                        if(temp3["title"].isNull() ||
                           temp3["url"].isNull() ||
                           temp3["img_urls"].isNull())
                        {
                            log.log(0, "title or url or img_urls is not exists");
                            log.log(0, content.asString());
                            continue;
                        }

                        title = temp3["title"].asString();
                        url = temp3["url"].asString();
                        for(int z = 0; (int) z < temp3["img_urls"].size(); z++){
                            const Json::Value temp4 = temp3["img_urls"][z];  
                            img = temp4.asString();
                            f = true;
                            break;
                        }
                        if(f) break;
                    }
                    if(f) break;
                }
                if(!f) continue;

                Json::Value r1, r2, r3, r4, r5;
                r5.append(img);
                r4["img_urls"] = r5;
                r4["title"] = title;
                r4["url"] = url;
                r3.append(r4);
                r2["news_lists"] = r3;
                r1.append(r2);

                Json::Value item;
                item["cardtag"] = content["result"]["cardtag"];
                item["desc"] = content["result"]["desc"];
                item["id"] = content["result"]["id"];
                item["name"] = content["result"]["name"];
                item["pic"] = content["result"]["pic"];
                item["url"] = content["result"]["url"];
                item["sub_contents"] = r1;
                item["keyword"] = details[j].rule + ":" + details[j].type;

                Json::FastWriter fastWriter;
                string id = fastWriter.write(content["result"]["id"]);
                log.log(1, string("doc id = " + id));

                if(duplicate.count(id)) {
                    VLOG(1) << "doc id =" + id + " exist, ignore it";
                    continue;
                }
                else {
                    duplicate.insert(id);
                    VLOG(1) << "doc id =" + id + " not exist, add it"; 
                }

                results.append(item);
 
                log.log(1, "Matching rule and content is found.");
                log.log(1, "Rule = " + details[j].rule);
                log.log(1, "Content = " + details[j].detail);
                log.log(1, "Deep = " + to_string(details[j].deep));

                rule = details[j].rule;
                type = details[j].type;

                ret = true;
                num++;
            }
        }
        else{
            log.log(1, "Matching rule and content is found, but is not active now.");
            log.log(1, "Rule = " + details[j].rule);
            log.log(1, "Content = " + details[j].detail);
            log.log(1, "Current Time:" + to_string((long)nt));
            log.log(1, "Doc start time:" + to_string((double)(details[j].starttime)));
            log.log(1, "Doc end time:" + to_string((double)(details[j].endtime)));
        }
    }

    if(!ret){
        Json::Value root;
        root["hasResults"] = "false";
        Json::FastWriter fast_writer;
        result = fast_writer.write(root);
        log.log(1, "All matching content is not avilable, in-active or jason parse failed.");
        return false;
    }
    else{
        Json::Value root;
        root["hasResults"] = "true";
        root["results"] = results;

        Json::FastWriter fast_writer;
        result = fast_writer.write(root);

        log.log(0, " query and matching rule = " + string("\t") + query + string("\t") + rule + string("\t") + type); 

        return true;
    }
}

void Storage::Clean(){
    //free acAutoKeyword 
    if(acAutoKeyword != NULL) delete(acAutoKeyword);

    //free Trie
    trie.clean();
}

//helper function start
bool Storage::ValidateString(string& input){
    boost::algorithm::trim(input);
    if(!strlen(input.c_str())) return false; else return true;
}

void Storage::ReleaseScroll(string& host, string& scrollId){
    string url = "http://" + host + "/_search/scroll?pretty"; 
    string postData = "{";
    postData += "\"scroll_id\":[\""+ scrollId +"\"]";
    postData += "}";

    VLOG(4) << "url: " << url;
    VLOG(4) << "PostData: " << postData;
    string output, errmsg;
    if(CurlWrapper::DeleteDataGetResponse(url, postData, &output, &errmsg)){ VLOG(0) << "Curl Delete scroll id failed"; } 
    VLOG(4) << "output: " << output;
    VLOG(4) << "errmsg: " << errmsg; 
    return;
}

long Storage::ParseTime(string& time){
    int year, month, day, hour, minute, second;
    if(sscanf(time.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6){
       struct tm t;
       t.tm_year = year-1900; 
       t.tm_mon = month-1; 
       t.tm_mday = day;
       t.tm_hour = hour;   
       t.tm_min = minute; 
       t.tm_sec = second;  
       return (long)mktime(&t);
    }
    else return 0;
}

void Storage::Log(){
    VLOG(2) << "logging words";
    for(int i = 0; i < (int)words.size(); i++){
        VLOG(2) << words[i];
    }

    VLOG(2) << "logging words mapping";
    map<string, int>::iterator it;
    for(it = wordsIdx.begin(); it != wordsIdx.end(); it++){
        VLOG(2) << it->first << "\t" << it->second; 
    }

    VLOG(2) << "logging rules";
    for(int i = 0; i < (int)rules.size(); i++){
        Rule* r = rules[i]; 
        string tr = "";
        bool b = true;
        for(int j = 0; j <(int)r->keywords.size(); j++){
            if(b){
                tr += words[r->keywords[j]];
                b = false;
            }
            else{
                tr += " & " + words[r->keywords[j]];
            }
        }
        VLOG(2) << "type: " << r->type;
        VLOG(2) << "rule: " << tr;
        VLOG(2) << "content: " << r->content;
        VLOG(2) << "st: " << r->st;
        VLOG(2) << "et: " << r->et;
    }
}

} //end namespace

