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
log = {}

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

    #logging(t)

    return

def offline(t):
    offlineUrl = "http://smart.sanjiaoshou.net/web/activityrecommend/automation?id=" + t.id + "&status=0"
    print("Offline Url", flush=True)
    print(offlineUrl, flush=True)
    #f = requests.get(url = offlineUrl)
    print("Took news " + t.id + " offline.", flush=True)
    return

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
    #print("Cnt = " + str(cnt))

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
        if(st > now or et <= now): continue

        diff = now - st         
        diff_days = diff.days
        if(diff_days < 1): 
            #print("st =  " + str(st))
            #print("now = " + str(now))
            #print(diff_days)
            continue

        autoUpdate = str(item["_source"]["autoUpdate"])
        if(autoUpdate.lower() != "true"): continue  

        t = topic()
        download(item, t)
        topics.append(t)

def load():

    global log

    f = open("hot_topic_format",  "r")
    for line in f:
        items = line.strip().split("\t")
        log[items[4]] = "1"
    f.close()

def purge():
    global topics
    global log  
    
    print("topic begin")
    for t in topics:
        print(t.keyword)     
    print("topic end")
    
    print("log begin")
    for l in log:
        print(l)     
    print("log end")

    print("missing start")
    for t in topics:
        if(t.keyword not in log):
            print(t.keyword)
    print("missing end")

if __name__ == '__main__':
    getNews()
    load()
    purge()

