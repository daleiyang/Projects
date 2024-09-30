# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
import requests
import json
import time
import sys
import hashlib
import datetime
import time
import base64

from pyhanlp import *
from operator import attrgetter
import operator
import urllib

#reload(sys)
#sys.setdefaultencoding('utf8')

from elasticsearch import Elasticsearch
es = Elasticsearch("http://172.16.0.213:28405")
#es = Elasticsearch("http://172.16.0.10:8405")
index = "highlight_news"
dataType = "external"

headers = {'User-Agent':'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Mobile Safari/537.36'}

TermFrequency = JClass('com.hankcs.hanlp.corpus.occurrence.TermFrequency')
TermFrequencyCounter = JClass('com.hankcs.hanlp.mining.word.TermFrequencyCounter')
TfIdfCounter = JClass('com.hankcs.hanlp.mining.word.TfIdfCounter')
TextRankKeyword = JClass("com.hankcs.hanlp.summary.TextRankKeyword")

chrome_headers = {'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8','Accept-Encoding': 'gzip, deflate, compress','Accept-Language':'en-us;q=0.5,en;q=0.3','Cache-Control':'max-age=0','Connection':'keep-alive','User-Agent':'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:22.0) Gecko/20100101 Firefox/22.0','Host':'www.baidu.com'}
url = "https://www.baidu.com/baidu?wd=XXX&tn=monline_dg&ie=utf-8"
res = []
log = {}
twoRecommandLog = []
twoDiscardLog = []
threeRecommandLog = []
threeDiscardLog = []
manualRecommandLog = []
manualDiscardLog = []

class news(object):
    def __init__(self):
        self.href = '' 
        self.title = '' 
        self.imgs = []
        self.source = ''
        self.time = ''
        self.type_ = 'news' #news or video 
        self.videotime = ''

class topic(object):
    def __init__(self):
        self.id = ''
        self.keyword = ''
        self.titles = []
        self.contents = []
        self.auto = []
        self.manual = []

topics = []
tempTopic = topic()

def logging(t):
    print("id:" + t.id, flush=True)
    print("keyword: " + t.keyword, flush=True)
    for i in range(len(t.titles)):
        print("title:" + t.titles[i], flush=True)
        print("content:" + t.contents[i], flush=True)
    if(len(t.auto) > 0): print("auto: " + str(t.auto), flush=True)
    if(len(t.manual) > 0): print("manual: " + str(t.manual), flush=True)
    print('\n', flush=True)

def download(item, t):
    t.id = str(item["_id"])
    t.keyword = item["_source"]["details"]["name"]
    items = item["_source"]["details"]["sub_contents"][0]["news_lists"] 
    rules = item["_source"]["normalized"]["name_lists"]
    flags = item["_source"]["normalized"]["flag"]
 
    for i in range(len(flags)):
        if(flags[i] == "auto"):
            t.auto.append(rules[i])
        if(flags[i] == "manual"):
            t.manual.append(rules[i])

    for i in range(len(items)):
        t.titles.append(items[i]["title"])
        t.contents.append(items[i]["content"])

    logging(t)

    return

def generate(t):
    #take news offline
    if(len(t.rules) == 0 and len(t.manualRecommand) == 0):
        offline(t)
        print("-------------------------------------------------------------------", flush=True)
        return 

    #update news
    if(len(t.rules) >= 0):
        update(t)
        print("-------------------------------------------------------------------", flush=True)
        return

def offline(t):
    offlineUrl = "http://smart.sanjiaoshou.net/web/activityrecommend/automation?id=" + t.id + "&status=0"
    print("Offline Url", flush=True)
    print(offlineUrl, flush=True)
    #f = requests.get(url = offlineUrl)
    print("Took news " + t.id + " offline.", flush=True)
    return

def update(t):
    newRules = [] 
    for r in t.rules:
        newRules.append(r + ":auto")
    for r in t.manual:
        newRules.append(r + ":manual")
    rules = ";".join(newRules)
    print(rules, flush=True)
    rules_bytes = rules.encode("utf-8")
    rules_base64 = base64.b64encode(rules_bytes)
    rules = str(rules_base64, "utf-8")
    updateUrl = "http://smart.sanjiaoshou.net/web/activityrecommend/upIntervene?id=" + t.id  + "&word=" + rules
    print("Update Url", flush=True)
    print(updateUrl, flush=True)
    f = requests.get(url = updateUrl)
    print("Updated news " + t.id + ".", flush=True)
    return

def find():
 
    global res
    global topics
    global twoRecommandLog
    global twoDiscardLog
    global threeRecommandLog
    global threeDiscardLog
    global manualRecommandLog
    global manualDiscardLog

    #input parameters
    freq = 4 
    tfidf = 4
    textRank = 4

    res = []
    class item: pass

    #create input
    num = 0
    for topic in topics:
        i = item()
        s = ''
        content = []
        titles = []

        i.title =topic.keyword 
        s += topic.keyword + '\n'
        content.append(topic.keyword)
        titles.append(topic.keyword)

        for idx in range(len(topic.titles)):
            s += topic.titles[idx] + '\n'
            #s += topic.contents[idx] + '\n'            
            content.append(topic.contents[idx])
            titles.append(topic.titles[idx])

        i.num = num
        i.text = s
        i.content = content
        i.titles = titles
        i.freq_num = freq
        i.tfidf_num = tfidf
        i.textRank_num = textRank
        i.oldauto = topic.auto
        i.manual = topic.manual
        res.append(i)    
        num += 1

    #freq
    for r in res:
        temp = set() 
        zz = TermFrequencyCounter.getKeywordList(r.text, int(freq))
        for z in zz:
            temp.add(z) 
        r.freq = temp

    #tfidf
    counter = TfIdfCounter()
    for r in res:
        counter.add(str(r.num), r.text)
    counter.compute()
 
    ts = [] 
    for id in counter.documents():
        i = item()  
        i.id = int(id)
        i.res = counter.getKeywordsOf(id, int(tfidf)).toString()
        ts.append(i)

    ts = sorted(ts, key=attrgetter('id'), reverse=False)
    for t in ts:
        #print(t.res)
        temp = set()
        items = t.res.replace("[", "").replace("]","").split(",") 
        for item in items:
            temp.add(item[0:item.index("=")])
        res[t.id].tfidf = temp

    #textRank
    for r in res:
        temp = set()
        zz = HanLP.extractKeyword(r.text, int(textRank)) 
        for z in zz:
            temp.add(z) 
        r.textRank = temp

    #union
    for r in res:
        u = set()
        for t in r.freq.union(r.tfidf.union(r.textRank)):
            u.add(t.strip())
        temp = []
        for t1 in u:
            temp.append(t1)
        r.union = temp  

    #remove single word
    idx = 0
    for r in res:
        r.oneKeywordCandidate = merge(r.union, r.titles)
        r.oneKeywordRecommand, r.twoKeywordsCandidate = checkOneKeyword(r.oneKeywordCandidate) 
        r.twoKeywordsRecommand, r.threeKeywordsCandidate = checkTwoKeywords(r.twoKeywordsCandidate, r.oneKeywordCandidate) 
        r.threeKeywordsRecommand = checkThreeKeywords(r.threeKeywordsCandidate, r.oneKeywordCandidate) 
        topics[idx].manualRecommand = checkManualKeywords(r.manual, r.oneKeywordCandidate) 

        print("<<专题名称>>" + "\n" + r.title + "\n", flush=True)
        print("<<新闻列表>>" + "\n" + r.text + "\n", flush=True)
        print("Freq = " + str(r.freq_num) + "\t" + str(r.freq), flush=True)
        print("Tfidf = " + str(r.tfidf_num) + "\t" + str(r.tfidf), flush=True)
        print("TextRank = " + str(r.textRank_num) + "\t" + str(r.textRank), flush=True)
        print("Union" + "\t" + str(r.union), flush=True)
        print("\n", flush=True)

        print("One Keyword Candidate" + "\t" + str(r.oneKeywordCandidate), flush=True)
        print("One Keyword Recommand", flush=True)
        for k in r.oneKeywordRecommand:
            print(log[k])
        print("One Keyword Discard", flush=True)
        for k in r.oneKeywordCandidate: 
            if k not in r.oneKeywordRecommand:
                print(log[k])
        print("\n", flush=True)

        print("Two Keywords Candidate" + "\t" + str(r.twoKeywordsCandidate), flush=True)
        print("Two Keywords Recommand", flush=True)
        for info in twoRecommandLog: print(info)
        twoRecommandLog = []
        print("Two Keywords Discard", flush=True)
        for info in twoDiscardLog: print(info)
        twoDiscardLog = []
        print("\n", flush=True)

        print("Three Keywords Candidate" + "\t" + str(r.threeKeywordsCandidate), flush=True)
        print("Three Keywords Recommand", flush=True)
        for info in threeRecommandLog: print(info)
        threeRecommandLog = []
        print("Three Keywords Discard", flush=True)
        for info in threeDiscardLog: print(info)
        threeDiscardLog = []
        print("\n", flush=True)

        print("Manual keywords Recommand", flush=True)
        for info in manualRecommandLog: print(info)
        manualRecommandLog = []
        print("Manual keywords Discard", flush=True)
        for info in manualDiscardLog: print(info)
        manualDiscardLog = []
        print("\n", flush=True)

        tt = ""
        if(len(r.oneKeywordRecommand) > 0): tt += str(r.oneKeywordRecommand)
        if(len(r.twoKeywordsRecommand) > 0): tt += str(r.twoKeywordsRecommand)
        if(len(r.threeKeywordsRecommand) > 0): tt += str(r.threeKeywordsRecommand)
        print("Old auto rules" + "\t" + str(r.oldauto), flush=True)
        print("\n", flush=True)
        print("Manual rules" + "\t" + str(r.manual), flush=True)
        print("\n", flush=True)

        rules = []
        if(len(r.oneKeywordRecommand) > 0): 
            for k in r.oneKeywordRecommand:
                rules.append(k)
        if(len(r.twoKeywordsRecommand) > 0):
            for ks in r.twoKeywordsRecommand:
                rules.append("&".join(ks))
        if(len(r.threeKeywordsRecommand) > 0):
            for ks in r.threeKeywordsRecommand:
                rules.append("&".join(ks))
        topics[idx].rules = rules 
        print("New auto rules" + "\t" + ";".join(topics[idx].rules), flush=True)
        print("\n", flush=True)

        #update topic
        generate(topics[idx])

        idx += 1

def merge(keywords, titles):
    res = []
    threshold = 2
    flag = []
    for i in range(len(keywords)): flag.append(0)
    for i in range(len(keywords)):
        if(len(keywords[i]) == 1 and flag[i] == 0):
            for j in range(len(keywords)):
                if(i == j or flag[j] == 1 or len(keywords[j]) > 1): continue
                t = keywords[i] + keywords[j]     
                cnt = 0
                for title in titles:
                    if t in title: cnt += 1
                if(cnt >= threshold):
                    res.append(t)
                    flag[i] = 1
                    flag[j] = 1
    for i in range(len(keywords)):
        if(len(keywords[i]) == 1): continue
        res.append(keywords[i])
    return res

def checkThreeKeywords(keywords, candidates):
    global threeRecommandLog
    global threeDiscardLog

    threshold_1 = 0.5
    threshold_2 = 0.5

    recommand = []
    for i in range(len(keywords)):
        info = ""
        t = keywords[i][0] + "%20" + keywords[i][1]  + "%20" + keywords[i][2]
        info += "Keyword = " + keywords[i][0] + " " + keywords[i][1]  + " " + keywords[i][2] + "\n" 
        res = search(t)     
        n = len(res)
        cnt = 0 
        for r in res:
            info += r + "\n"
            sum = len(candidates)
            f = 0
            for c in candidates: 
                if(c in r): f += 1
            ra = 0.0
            if(sum > 0): ra = (1.0*f)/(1.0*sum)
            if(ra > threshold_1): cnt += 1
        info += "Sum = " + str(n) + "\n"
        info += "Cnt = " + str(cnt) + "\n"
        ratio = 0.0
        if(n > 0): ratio = (1.0*cnt)/(1.0*n) 
        info += "Ratio = " + str(ratio) + "\n"
        if(ratio > threshold_2):
            tt = []
            tt.append(keywords[i][0])
            tt.append(keywords[i][1])
            tt.append(keywords[i][2])
            recommand.append(tt)  
            threeRecommandLog.append(info)
        else:
            threeDiscardLog.append(info)

    return recommand

def checkTwoKeywords(keywords, candidates):
    global twoRecommandLog
    global twoDiscardLog

    threshold_1 = 0.5
    threshold_2 = 0.5

    t_c = []
    recommand = []
    for i in range(len(keywords)):
        for j in range(i+1, len(keywords)):
            info = ""
            t = keywords[i] + "%20" + keywords[j]
            info += "Keyword = " + keywords[i] + " " + keywords[j] + "\n"
            res = search(t)    
            n = len(res)
            cnt = 0 
            for r in res:
                info += r + "\n"
                sum = len(candidates)
                f = 0
                for c in candidates: 
                    if(c in r): f += 1
                ra = 0.0
                if(sum > 0): ra = (1.0*f)/(1.0*sum)
                if(ra > threshold_1): cnt += 1
            info += "Sum = " + str(n) + "\n"
            info += "Cnt = " + str(cnt) + "\n"
            ratio = 0.0
            if(n > 0): ratio = (1.0*cnt)/(1.0*n) 
            info += "Ratio = " + str(ratio) + "\n"
            if(ratio > threshold_2):
                tt = []
                tt.append(keywords[i])
                tt.append(keywords[j])
                recommand.append(tt)  
                twoRecommandLog.append(info)
            else:
                for k in range(len(keywords)):
                    if(k == i or k == j): continue
                    c = []
                    c.append(keywords[i])
                    c.append(keywords[j])
                    c.append(keywords[k])
                    c.sort()
                    t_c.append(c)
                twoDiscardLog.append(info)                
 
    #remove three candidate which has two keywords recommand
    t_t = []
    for c in t_c:
        f_1 = False
        for r in recommand:
            f_2 = True
            for k in r:
                if k not in c: 
                    f_2 = False
                    break
            if(f_2 == True): 
                f_1 = True 
                break
        if(f_1 == False): t_t.append(c)

    #remove duplicate records
    t_t.sort()
    candidate = []
    pre = 0
    for i in range(len(t_t)):
        if(i == 0): 
            candidate.append(t_t[0])
        else:
            if(candidate[pre]  == t_t[i]): continue
            else:
                candidate.append(t_t[i])
                pre = len(candidate)-1

    return recommand, candidate

def checkOneKeyword(keywords):
    global log

    threshold_1 = 0.5
    threshold_2 = 0.5

    recommand = []
    flag = []
    for i in range(len(keywords)): flag.append(0)

    for i in range(len(keywords)):
        info = ""
        info += "Keyword = " + keywords[i] + "\n"
        res = search(keywords[i])     
        n = len(res)
        cnt = 0 
        for r in res:
            info += r + "\n"
            sum = len(keywords)
            f = 0
            for j in range(sum): 
                if(keywords[j] in r): f += 1
            ra = 0.0
            if(sum >0): ra = (1.0*f)/(1.0*sum)
            if(ra > threshold_1): cnt += 1
        info += "Sum = " + str(n) + "\n"
        info += "Cnt = " + str(cnt) + "\n"
        ratio = 0.0
        if(n > 0): ratio = (1.0*cnt)/(1.0*n) 
        info += "Ratio = " + str(ratio) + "\n"
        if(ratio > threshold_2):
            flag[i] = 1
            recommand.append(keywords[i])  
        log[keywords[i]] = info

    candidate = []
    for i in range(len(flag)):
        if(flag[i] == 0): 
            candidate.append(keywords[i])

    return recommand, candidate

def checkManualKeywords(manual, candidates):
    global manualRecommandLog
    global manualDiscardLog

    threshold_1 = 0.5
    threshold_2 = 0.5

    recommand = []

    for i in range(len(manual)):
        info = ""
        keywords = manual[i].split('&')
        t = ""
        for idx in range(len(keywords)):
            if(idx < len(keywords) - 1):
                t += keywords[idx] + "%20" 
            else:
                t += keywords[idx]
        info += "Keyword = " + t + "\n"
        res = search(t)     
        n = len(res)
        cnt = 0 
        for r in res:
            info += r + "\n"
            sum = len(candidates)
            f = 0
            for j in range(sum): 
                if(candidates[j] in r): f += 1
            ra = 0.0
            if(sum >0): ra = (1.0*f)/(1.0*sum)
            if(ra > threshold_1): cnt += 1
        info += "Sum = " + str(n) + "\n"
        info += "Cnt = " + str(cnt) + "\n"
        ratio = 0.0
        if(n > 0): ratio = (1.0*cnt)/(1.0*n) 
        info += "Ratio = " + str(ratio) + "\n"
        if(ratio > threshold_2):
            recommand.append(manual[i])  
            manualRecommandLog.append(info)
        else:
            manualDiscardLog.append(info)

    return recommand

def search(keyword):
    rs = []
    u = url.replace('XXX', urllib.parse.quote(keyword))
    retry = 0 
    while retry < 10:
        try:
            req = requests.get(url=u, headers=chrome_headers)
            break
        except:
            print("retry = " + str(retry))
            retry += 1
            time.sleep(1)
    res = req.content
    div_bf = BeautifulSoup(res, features='html.parser')
    items = div_bf.find_all('div', class_ = 'result c-container')
    for item in items:
        title = item.find('h3').text
        rs.append(title)

    time.sleep(1)
    return rs 

def getNews():

    global topics

    detail = '{\"query\": {\"match_all\": {}}, \"size\":1}'
    res = es.search(index=index, doc_type=dataType, body=detail)
    obj = json.dumps(res)
    obj2 = json.loads(obj)
    cnt = obj2["hits"]["total"]
    if(cnt == 0): 
        print("No news exists in ES, ignore it", flush=True)
        return False
    print("Cnt = " + str(cnt))

    time.sleep(2)

    detail = '{\"query\": {\"match_all\": {}}, \"size\":' + str(cnt) + '}'
    res = es.search(index=index, doc_type=dataType, body=detail)
    obj = json.dumps(res)
    obj2 = json.loads(obj)
    items = obj2["hits"]["hits"]
    for item in items:
        if("newsType" not in item["_source"]): continue
        newsType = str(item["_source"]["newsType"])
        if(newsType.lower() != '热点新闻'): continue
        sts = str(item["_source"]["starttime"])
        st = datetime.datetime.strptime(sts, '%Y-%m-%d %H:%M:%S')
        ets = str(item["_source"]["endtime"])
        et = datetime.datetime.strptime(ets, '%Y-%m-%d %H:%M:%S')
        now = datetime.datetime.now()
        #upper = datetime.datetime.strptime('2020-05-31 00:00:00', '%Y-%m-%d %H:%M:%S')
        #lower = datetime.datetime.strptime('2020-05-28 00:00:00', '%Y-%m-%d %H:%M:%S')
        #if(st > now): continue
        #if(et < lower): continue
        #if(et > upper): continue
        if(st > now or et <= now): continue
        autoUpdate = str(item["_source"]["autoUpdate"])
        if(autoUpdate.lower() != "true"): continue  

        t = topic()
        download(item, t)
        topics.append(t)

if __name__ == '__main__':
    #download topics from ES
    getNews()

    #find keywords
    find()

