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
#es = Elasticsearch("http://172.16.0.10:8405")
es = Elasticsearch("http://172.16.0.213:28405")
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
    #print(item)
    t.id = str(item["_id"])
    t.keyword = item["_source"]["details"]["name"]

    if(t.keyword == "热点新闻监控1"): return False
    if(len(item["_source"]["details"]["sub_contents"]) == 0): return False
    
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

    return True

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
        #print(item)
        #check news type
        if("newsType" not in item["_source"]): continue
        newsType = str(item["_source"]["newsType"])
        #if(newsType.lower() == '热点新闻'): continue

        #check start time
        if "starttime" not in item["_source"]: continue
        sts = str(item["_source"]["starttime"])
        if(sts == "None"): continue
        #print("sts " + sts)
        st = datetime.datetime.strptime(sts, '%Y-%m-%d %H:%M:%S')
        
        #check end time
        if "endtime" not in item["_source"]: continue
        ets = str(item["_source"]["endtime"])
        if(ets == "None"): continue
        #print("ets " + ets)
        et = datetime.datetime.strptime(ets, '%Y-%m-%d %H:%M:%S')

        now = datetime.datetime.now()
        if(st > now or et <= now): continue

        t = topic()
        if(download(item, t) == False): continue
        topics.append(t)

    for t in topics:
        print(t.keyword + '\t' + str(t.manual) + '\t' + str(t.auto))

if __name__ == '__main__':
    #download topics from ES
    getNews()

