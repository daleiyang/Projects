#include "Storage.h"

DEFINE_int32(time_out, 5000, "http requests time out" );
DEFINE_string(tmall_api,"https://bombonera-dsp.taobao.com/cpm/ask","");

//QueryPreProcess
DEFINE_string(QueryPreProcessDir, "../Data/Parser", "");
DEFINE_string(QueryPreProcessPunct, "punct.dic","");
DEFINE_string(QueryPreProcessEnRecognizer, "","");
DEFINE_string(QueryPreProcessNumRecognizer,"num.dic","");
DEFINE_string(QueryPreProcessSpecPunct,"spec_punct.dic","");

//Taobao_Api_Enable
DEFINE_bool(taobao_api_enable, true, "enable taobao api enable");

namespace robot {

Storage::Storage(){ }

Storage::~Storage(){ }

bool Storage::Init(TmallShoppingContentPlugin* t){
    //init white list
    string whiteListFile = "../Data/white_list.txt";
    file::SimpleLineReader reader(whiteListFile.c_str(), true);
    vector<string> lines;
    reader.ReadLines(&lines);

    for(int i=0; i < (int)lines.size(); i++){
        vector<string> vname;
        SplitString(lines[i], '\t', &vname);
        if(vname.size() != 2) {
            VLOG(0) << lines[i] << " format is wrong, ignore it."; 
            continue;
        }
        string lower = boost::algorithm::to_lower_copy(vname[0]);
        whiteList.insert(make_pair(lower, vname[1]));
    }
    VLOG(0) << "White list loaded successfully.";

    //init cnn
    if(!InitCnn()) return false;

    //init TmallShoppingContentPlugin
    tscp = t;

    return true;
}

bool Storage::InitCnn(){
    //init cnn          
    vector<string> inputs;
    inputs.push_back("input_x");
    inputs.push_back("keep_prob");

    vector<string> outputs;
    outputs.push_back("score/y_prob");

    string model = "../Data/cnn/graph.pb";
    string ckpt = "../Data/cnn/best_validation";

    wrapper = new TFCWrapper();
    if (!wrapper->InitWithCKPT(model, ckpt, inputs, outputs)) {
        VLOG(0) << "TFCWrapper init failed";
        return false;
    } 
    VLOG(0) << "Tensorflow C API init successfully."; 

    //init word dic
    string wordIdFile = "../Data/cnn/vocab.txt";
    file::SimpleLineReader reader(wordIdFile.c_str(), true);
    vector<string> lines;
    reader.ReadLines(&lines);

    wordId = new map<string, int>();
    int idx = 0;
    for(int i = 0; i < (int)lines.size(); i++){
        boost::algorithm::trim(lines[i]);
        (*wordId)[lines[i]] = idx++;
    }
    VLOG(0) << "Word dictionary loaded successfully."; 

    //init category list
    string categoryFile = "../Data/cnn/category.txt";
    file::SimpleLineReader reader2(categoryFile.c_str(), true);
    vector<string> lines2;
    reader2.ReadLines(&lines2);

    category = new vector<string>();
    for(int i = 0; i < (int)lines2.size(); i++){
        boost::algorithm::trim(lines2[i]);
        category->push_back(lines2[i]); 
    }
    VLOG(0) << "Category list loaded successfully.";

    return true;
}

//Not a member function, just for pthread_create() usage.
void * LaunchWorker(void * obj){
    Launcher * launcher = reinterpret_cast<Launcher *>(obj);
    return launcher->launch();
}

void * Storage::Worker(Parameter* pa){
    unsigned long long diff; 
    struct timespec start, end;

    if(pa->type == "JD") {
        clock_gettime(CLOCK_MONOTONIC, &start);
        pa->success = JDAPI(pa->query, pa->wareNames, pa->imageUrls, pa->prices, pa->skuIds); 
        clock_gettime(CLOCK_MONOTONIC, &end);

        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        VLOG(3) << "API call time cost log" << "\t" << "JDAPI" << "\t" << (double)diff / BILLION;
        VLOG(3) << "API call result" << "\t" << "JDAPI" << "\t" << pa->success;
    }
    if(pa->type == "Tmall") {
        clock_gettime(CLOCK_MONOTONIC, &start);
        pa->success = TmallProductApi(pa->query, pa->deviceId, pa->result);
        clock_gettime(CLOCK_MONOTONIC, &end);

        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        VLOG(3) << "API call time cost log" << "\t" << "TmallProductApi" << "\t" << (double)diff / BILLION;
        VLOG(3) << "API call result" << "\t" << "TmallProductApi" << "\t" << pa->success;
    }
    if(pa->type == "Brand") {
        clock_gettime(CLOCK_MONOTONIC, &start);
        pa->success = TmallBrandApi(pa->query, pa->result);
        clock_gettime(CLOCK_MONOTONIC, &end);

        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        VLOG(3) << "API call time cost log" << "\t" << "TmallBrandApi" << "\t" << (double)diff / BILLION;
        VLOG(3) << "API call result" << "\t" << "TmallBrandApi" << "\t" << pa->success;
    }
    return 0;
}

bool Storage::Search(string& traceId, string& userId, string& deviceId, string& query, string& result){

    // normalize query
    string normalizedQuery = "";
    VLOG(3) << "Query: " << query;
    if(!FrameSemantic::formatString(query, normalizedQuery )){
       VLOG(0) << "FormatString error. value: " << query;
       return false;
    }
    VLOG(3) << "Normalized Query: " << normalizedQuery;

    //Need to find out which way to go
    bool Brand = false;
    bool Taobao = false;
    bool JD = false;

    //White list
    string cls="";
    string lower = boost::algorithm::to_lower_copy(query);
    if(whiteList.count(lower))
        cls = whiteList[lower];

    string tmp_result;

    unsigned long long diff; 
    struct timespec start, end;
    //If hit whitelist and is phone
    if(cls == "1"){
        VLOG(3) << query << " hits white list and is phone.";
        Brand = true;
        Taobao = false;
        JD = false;

        clock_gettime(CLOCK_MONOTONIC, &start);
        bool ret = TmallBrandApi(query, tmp_result);
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        VLOG(3) << "API call time cost log" << "\t" << "TmallBrandApi" << "\t" << (double)diff / BILLION;
        VLOG(3) << "API call result" << "\t" << "TmallBrandApi" << "\t" << ret;

        if(!ret){
            VLOG(0) << "Brand call failed.";
            return false;
        }
        VLOG(3) << "Brand call result:" << tmp_result;
    }
    //If hit whitelist and is not phone and taobao api is enabled
    else if(FLAGS_taobao_api_enable && (cls == "2" || cls == "3" || cls == "4")){
        VLOG(3) << query << " hits white list and is not phone.";
        Brand = false; 
        Taobao = true;
        JD = false;

        clock_gettime(CLOCK_MONOTONIC, &start);
        bool ret = TmallProductApi(query, deviceId, tmp_result);
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        VLOG(3) << "API call time cost log" << "\t" << "TmallProductApi" << "\t" << (double)diff / BILLION;
        VLOG(3) << "API call result" << "\t" << "TmallProductApi" << "\t" << ret;

        if(!ret){
            VLOG(0) << "Tmall call failed.";
            return false;
        }
        VLOG(3) << "Tmall call result:" << tmp_result;
    }
    //Not in white list or taobao api os disable, relay on JD title and CNN to classify
    else{
        VLOG(3) << query << " not in white list, relay on JD title and CNN to classify.";
        //Multithread call JD & Tmall & Brand
        //For JD call
        Parameter* pa_1 = new Parameter("JD", query);
        Launcher* la_1 = new Launcher(this, pa_1);
        pthread_t* pt_1 = new pthread_t();

        //For Tmall call
        Parameter* pa_2 = NULL;
        Launcher* la_2 = NULL;
        pthread_t* pt_2 = NULL;
        if(FLAGS_taobao_api_enable){
            pa_2 = new Parameter("Tmall", deviceId, query);
            la_2 = new Launcher(this, pa_2);
            pt_2 = new pthread_t();
        }
 
        //For Brand
        Parameter* pa_3 = new Parameter("Brand", query);
        Launcher* la_3 = new Launcher(this, pa_3);
        pthread_t* pt_3 = new pthread_t();

        //Start threads
        pthread_create(pt_1, NULL, LaunchWorker, la_1);
        if(FLAGS_taobao_api_enable){
            pthread_create(pt_2, NULL, LaunchWorker, la_2);
        }
        pthread_create(pt_3, NULL, LaunchWorker, la_3);
        VLOG(3) << "All threads started.";

        //Wait for finish
        pthread_join(*pt_1, NULL);
        if(FLAGS_taobao_api_enable){
            pthread_join(*pt_2, NULL);
        }
        pthread_join(*pt_3, NULL);
        VLOG(3) << "All threads finished.";

        //Collect result
        bool JDSucc = pa_1->success; 
        vector<string> wareNames = pa_1->wareNames;
        vector<string> imageUrls = pa_1->imageUrls;
        vector<string> prices = pa_1->prices;
        vector<string> skuIds = pa_1->skuIds;

        bool TmallSucc = false;
        string TmallRet = "";
        if(FLAGS_taobao_api_enable){
            TmallSucc = pa_2->success; 
            TmallRet = pa_2->result; 
        }

        bool BrandSucc = pa_3->success;
        string BrandRet = pa_3->result;

        //Free Resource
        delete pa_1;
        delete la_1;
        delete pt_1;

        if(FLAGS_taobao_api_enable){
            delete pa_2;
            delete la_2;
            delete pt_2;
        }

        delete pa_3;
        delete la_3;
        delete pt_3;

        //If get JD title failed, we need to end this call 
        if(!JDSucc) {
            VLOG(0) << "JD call failed.";
            return false;
        }

        //No results returned from JD, we relay on tmall results
        if(!wareNames.size()){
            if(FLAGS_taobao_api_enable && TmallSucc) {
                Brand = false; 
                Taobao = true;
                JD = false;
                tmp_result = TmallRet;
            }
            else{
                VLOG(0) << "JD return 0 titles and Tmall call failed.";
                return false;
            } 
        }
        //Relay on CNN to identify Brand or Tmall
        else{
            //Infer type
            vector<float> score(category->size());
            for(int i = 0; i < (int)score.size(); i++) score[i] = 0.0;

            for(int i = 0; i < min((int)(wareNames.size()), 10); i++){
                vector<float> prob;
                Inference(wareNames[i], prob);    
                for(int j = 0; j < (int)prob.size(); j++) score[j] += prob[j];
            }

            for(int i = 0; i < (int)score.size(); i++){
                score[i] = score[i] / (int)(wareNames.size()); 
                VLOG(3) << category->at(i) << " : " << score[i]; 
            }
 
            int idx = 0; float max = -1.0;
            for(int i = 0; i < (int)score.size(); i++){
                if(score[i] > max){max = score[i]; idx = i;}
            }
            if(idx == 1){
                Brand = true; 
                Taobao = false;
                JD = false;
                if(BrandSucc) { tmp_result = BrandRet; }
                else{
                    VLOG(0) << "Brand call failed.";
                    return false;
                }
                VLOG(3) << "Brand call result:" << tmp_result;
            }
            if(FLAGS_taobao_api_enable && (idx == 0 || idx == 3 || idx ==2)){
                Brand = false; 
                Taobao = true;
                JD = false;
                if(TmallSucc) { tmp_result = TmallRet; }
                else{
                    VLOG(0) << "Tmall call failed.";
                    return false;
                }
            }
            //return jd result with taobao format
            if(!FLAGS_taobao_api_enable && (idx == 0 || idx == 3 || idx ==2)){
                Brand = false; 
                Taobao = false;
                JD = true;

                string res = "{";
                if(!wareNames.size()){
                    res += "\"result\":false,"; 
                    res += "\"data\":[],"; 
                    res += "\"type\":null"; 
                }
                else{
                    res += "\"result\":true,"; 
                    res += "\"data\":[";
                    for(int i = 0; i < (int)wareNames.size(); i++){
                        res += "{";  
                        res += "\"title\":\"" + wareNames[i]  + "\",";
                        res += "\"picUrl\":\"" + imageUrlBase + imageUrls[i]  + "\",";
                        res += "\"price\":\"" + prices[i]  + "\",";
                        res += "\"h5Url\":\"" + itemUrlBase + skuIds[i] + ".html"  + "\",";
                        res += "\"deeplinkUrl\":\"" + baseIntentUrlPre  + skuIds[i]  + baseIntentUrlPost + "\"";
                        res += "}";
                        if(i < wareNames.size()-1){
                            res += ",";   
                        }
                    }
                    res += "],"; 
                    res += "\"type\":null"; 
                }
                res += "}";
                result = res;
                VLOG(3) << "Final result:" << result;
                return true;
            }
        }
    }

    string query_type;
    if(Brand){ query_type = "Brand"; VLOG(3) << "Brand"; }
    if(Taobao){ query_type = "TB"; VLOG(3) << "Taobao"; }

    result = "";
    GenerateFinalResult(query_type, tmp_result, result);
    VLOG(3) << "Final result:" << result;

    return true; 
}

bool Storage::JDAPI(string& query, vector<string>& wareNames, vector<string>& imageUrls, vector<string>& prices, vector<string>& skuIds){
    //time
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[128];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 128, "%Y-%m-%d %H:%M:%S", timeinfo);

    //All parameters
    map<string, string> args; 
    args["method"] = "jd.kepler.xuanpin.search.sku"; // API接口名称
    args["method_param"] = "{\"orderField\":0,\"pageParam\":{\"pageNum\":1,\"pageSize\":10},\"queryParam\":{\"keywords\":\"" + query + "\",\"sellerType\":2}}";
    args["po_app_key"] = "7303d11bf6d4475fae1096ad5c8a5792"; //分配给厂商的AppKey
    args["api_type"] = "2";     //平台接口类型，默认值是2
    args["timestamp"] = string(buffer);     //时间戳
    args["format"] = "json";    //响应格式。暂时只支持json
    args["v"] = "1.2";          //API协议版本，可选值：1.0
    args["sign_method"] = "md5";//签名的摘要算法，可选值为：md5
    args["sign"] = "";          //API输入参数签名结果
    string appSercet = "98ce8195de764efc9bd41c15b22abb76";

    //Generate raw data to calculate MD5
    string raw = appSercet;
    map<string, string>::iterator it;
    for(it = args.begin(); it != args.end(); it++){
        if(it->first != "sign" and it->first != "api_type"){
            raw += it->first;
            raw += it->second;
        }
    }
    raw += appSercet;
 
    //MD5 SUM
    MD5_CTX hash_ctx;
    unsigned char hash_ret[16];

    MD5_Init(&hash_ctx);
    MD5_Update(&hash_ctx, raw.c_str(), strlen(raw.c_str()));
    MD5_Final(hash_ret, &hash_ctx);
 
    char res[32];
    for(int i = 0; i < 32; i++){
        if(i % 2 == 0) sprintf(res+i, "%x", (hash_ret[i/2] >> 4) & 0xf);
        else sprintf(res+i, "%x", (hash_ret[i/2]) & 0xf);
    }
    for(int i = 0; i< 32; i++) res[i] = toupper(res[i]);
    args["sign"] = string(res);

    //Generate http request
    string request = "https://winservice.jd.com/api?";
    string splitSymbol = "";
    for(it = args.begin(); it != args.end(); it++){
        request += splitSymbol;
        splitSymbol = "&";
        request += it->first;
        request += "=";
        if(it->first == "method_param" or it->first =="timestamp"){
            string encoded;
            encode::UrlEncodeString(it->second, &encoded);
            request += encoded;
        }
        else request += it->second;
    }
    VLOG(4) << "Request URL";
    VLOG(4) << request;

    string post = "{}", output, errmsg; 
    if(CurlWrapper::GetRespondData(request, FLAGS_time_out, &output, &errmsg)){
        VLOG(0) << "Curl Get data failed.";
        VLOG(0) << "URL: " << request;
        VLOG(0) << "Output: " << output;
        VLOG(0) << "Error Message: " << errmsg;
        return false;
    }
    VLOG(4) << "Output:" << output;
    VLOG(4) << "Error Message:" << errmsg;

    Json::Reader reader(Json::Features::strictMode());
    Json::Value root;
    if(!reader.parse(output, root)){ 
        VLOG(0) << "parse json request failed"; 
        return false;
    }

    if(root["body"].isNull() ||
       root["body"]["jd_kepler_xuanpin_search_sku_response"].isNull() ||
       root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"].isNull() ||
       root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["totalNum"].isNull() ||
       root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"].isNull())
    {
        VLOG(0) << "JD's result is not correct.";
        return false;
    }
    int totalNum = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["totalNum"].asInt();
    if(!totalNum) return true;

    int size = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"].size();
    for(int i = 0; i < size; i++){
        string wareName = "";
        if(!root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["wareName"].isNull()){
            wareName = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["wareName"].asString(); 
        }
        string imageUrl = "";
        if(!root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["imageUrl"].isNull()){
            imageUrl = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["imageUrl"].asString(); 
        }
        double price = 0.0;
        if(!root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["price"].isNull()){
            price = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["price"].asDouble(); 
        }
        double skuId = 0;
        if(!root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["skuId"].isNull()){
            skuId = root["body"]["jd_kepler_xuanpin_search_sku_response"]["skuList"]["list"][i]["skuId"].asDouble(); 
        }

        wareNames.push_back(wareName);
        imageUrls.push_back(imageUrl);
        prices.push_back(to_string(price));
        skuIds.push_back(to_string(skuId));
    } 
 
    return true;
}

bool Storage::Inference(string& input, vector<float>& prob){
    const char* c = input.c_str();
    VLOG(3) << "Inference input is: " << c;

    vector<int> ids;
    char buffer[1024];
    int idx = 0, step = 0;
    int len = strlen(c);  
    while(idx < len){
        step = FindCharSize(c, idx); 
        strncpy(buffer, c+idx, step);
        buffer[step] = 0;
        idx += step;
 
        string s = string(buffer);
        if(wordId->count(s)) 
            ids.push_back((*wordId)[s]);
        else 
            ids.push_back(1);
    }

    //input
    std::vector<TFCWrapper::InputValue> ivalues;

    ivalues.push_back(TFCWrapper::InputValue());
    ivalues.back().type_ = TF_INT32;
    int64_t dims[] = {1, 600};
    ivalues.back().dims_ = dims;
    ivalues.back().num_dims_ = 2;
    int32_t datas[600] = {0};  
    for(int i = 0; i < min(600, (int)ids.size()); i++) datas[i] = ids[i];    
    ivalues.back().data = datas;
    ivalues.back().len = 600 * sizeof(int32_t);

    //dropout
    ivalues.push_back(TFCWrapper::InputValue());
    ivalues.back().type_ = TF_FLOAT;
    int64_t dims2[] = {1, 1};
    ivalues.back().dims_ = dims2;
    ivalues.back().num_dims_ = 2;
    float datas2[] = {1.0};
    ivalues.back().data = datas2;
    ivalues.back().len = 1 * sizeof(float);

    //probs
    std::vector<TFCWrapper::OutputValue> ovalues;

    ovalues.push_back(TFCWrapper::OutputValue());
    ovalues.back().type_ = TF_INT32;
    int64_t odims[] = {1, 4};
    ovalues.back().dims_ = odims;
    ovalues.back().num_dims_ = 2;
    float odatas[] = {0.0, 0.0, 0.0, 0.0};
    ovalues.back().data = odatas;
    ovalues.back().len = 4 * sizeof(float);
 
    if (!wrapper->Run(ivalues, ovalues)) {
        VLOG(0) << "[-NaN -NaN] failed";
        return false;
    } else {
        for(int i = 0; i < (int)category->size(); i++){
            float f = ((float*)ovalues.back().data)[i];
            prob.push_back(f);
        }
        return true;
    }
}

int Storage::FindCharSize(const char* s, int idx){
    int len = strlen(s+idx);
    int cnt;
    if((s[idx] & 0xFC) == 0xFC && len >= 6) cnt = 6;
    else if((s[idx] & 0xF8) == 0xF8 && len >= 5) cnt = 5;
    else if((s[idx] & 0xF0) == 0xF0 && len >= 4) cnt = 4;
    else if((s[idx] & 0xE0) == 0xE0 && len >= 3) cnt = 3;
    else if((s[idx] & 0xC0) == 0xC0 && len >= 2) cnt = 2;
    else cnt = 1;
    return cnt;
}

bool Storage::TmallProductApi(string& query, string& deviceId, string& result){
    /*
    string encode_query;
    encode::UrlEncodeString(query, &encode_query);
    VLOG(4) << "QUERY: " << query;
    VLOG(4) << "ENCODE QUERY: " << encode_query;
    */

    string url = "http://bombonera-dsp.taobao.com/cpm/ask";
    string post_content = "channel=4&adid=2182&source=oppo&imeiMd5="+ deviceId +"&keywordType=2&keyword=" + query;
    VLOG(4) << "post_content:" << post_content;

    map <string, string> headers;
    headers["Content-type"] = "Content-type: application/x-www-form-urlencoded";

    string error = "";
    int resp_level = common::CurlWrapper::GetPostRespondData(url, post_content, headers, FLAGS_time_out, &result, &error);

    if(0 != resp_level || !error.empty()){
        VLOG(0) << "Error Info: " << error;
        VLOG(0) << "Response Code: " << resp_level;
        return false;
    }
    VLOG(4) << "TAOBAO API RESULT: " << result;

    return true;
}

bool Storage::TmallBrandApi(string& query, string& result){
    bool ret = tscp->TmallBrand(query, result);
    return ret;
}

bool Storage::GenerateFinalResult(string& qtype, string& type_result, string& result){
    Json::Value tmp_result;
    Json::Reader reader(Json::Features::strictMode());
    if(!reader.parse(type_result, tmp_result)){
        VLOG(0) << "parse json request failed";
        return false;
    }

    Json::Value final_result;
    Json::Value product;
    int top = 0;

    if(qtype == "Brand"){
        final_result["type"] = 1;
        if(tmp_result["status"].asString() == "success"){
            final_result["shop"].append(tmp_result["result_list"][top]["details"]);
        }
        else final_result["type"] = -1;
    }
    else if ( qtype == "TB" )
    {
        final_result["type"] = 0;

        if( !tmp_result["result"].asBool() )
        {
            final_result["type"] = -1;
        }
        else if ( tmp_result["data"].size() == 0 )
        {
            final_result["type"] = -1;
        }
        else
        {
            product["result"] = true;
            final_result["product"].append(tmp_result["data"][top]);
        }
    }

    Json::FastWriter writer;
    result = writer.write(final_result);
    return true;
}

void Storage::Clean(){
    delete wrapper;
    delete wordId;
    delete category;
}

bool Storage::CheckJDAPI(string& query, string& result){
    vector<string> wareNames;
    vector<string> imageUrls;
    vector<string> prices;
    vector<string> skuIds;
    bool ret = JDAPI(query, wareNames, imageUrls, prices, skuIds);     
    for(int i = 0; i < (int)wareNames.size(); i++) result += wareNames[i] + "\n";
    for(int i = 0; i < (int)imageUrls.size(); i++) result += imageUrls[i] + "\n";
    for(int i = 0; i < (int)prices.size(); i++) result += prices[i] + "\n";
    for(int i = 0; i < (int)skuIds.size(); i++) result += skuIds[i] + "\n";
    return ret;
}

bool Storage::CheckTmallAPI(string& query, string& deviceId, string& result){
    return TmallProductApi(query, deviceId, result);
}

bool Storage::CheckBrandAPI(string& query, string& result){
    return TmallBrandApi(query, result);
}

} //end namespace

