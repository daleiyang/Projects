# -*- coding: utf-8 -*-
from bs4 import BeautifulSoup
import requests
import json
import time
import sys
import hashlib
import datetime
import time
import sys

from pyhanlp import *
from operator import attrgetter
import operator
import urllib

#reload(sys)
#sys.setdefaultencoding('utf8')

from elasticsearch import Elasticsearch
es = Elasticsearch("http://172.16.0.41:8405")
index = "highlight_news_recommend_yidian"
dataType = "external"

headers = {'User-Agent':'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.88 Mobile Safari/537.36'}
#news_url = 'https://www.yidianzixun.com/home/q/hot_search_keywords?appid=yidian&_=XXX'
news_url = 'https://www.yidianzixun.com/open-platform/get_hot_topic?appid=yidian&timestamp=XXX'
detail_url = 'https://www.yidianzixun.com/channel/w/XXX'

TermFrequency = JClass('com.hankcs.hanlp.corpus.occurrence.TermFrequency')
TermFrequencyCounter = JClass('com.hankcs.hanlp.mining.word.TermFrequencyCounter')
TfIdfCounter = JClass('com.hankcs.hanlp.mining.word.TfIdfCounter')
TextRankKeyword = JClass("com.hankcs.hanlp.summary.TextRankKeyword")

chrome_headers = {'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8','Accept-Encoding': 'gzip, deflate, compress','Accept-Language':'en-us;q=0.5,en;q=0.3','Cache-Control':'max-age=0','Connection':'keep-alive','User-Agent':'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:22.0) Gecko/20100101 Firefox/22.0','Host':'www.baidu.com'}
url = "https://www.baidu.com/baidu?wd=XXX&tn=monline_dg&ie=utf-8"
res = []
log = {}

class news(object):
    def __init__(self):
        self.href = '' 
        self.title = '' 
        self.content = ''
        self.imgs = []
        self.source = ''
        self.time = ''
        self.type_ = 'news' #news or video 
        self.videotime = ''

class topic(object):
    def __init__(self):
        self.title_name = ''
        self.title_img = ''
        self.news = []
        self.keyword = ''

topics = []
tempTopic = topic()

def logging(t):
    print("Title name: " + t.title_name)
    print("Title image: " + t.title_img)
    print('\n')
    for n in t.news:
        print('Href: ' + n.href)
        print('Type: ' + n.type_)
        if(n.type_ == 'video'): print('Video Time: ' + n.videotime)
        print('Title: ' + n.title)
        print('Content: ' + n.content)
        for img in n.imgs:
            print('Images = ' + img)
        print('Source: ' + n.source)
        print('Time: ' + n.time)
        print('\n')

def downloadContent(url):
    print("Download content from " + url)
    content = ''
    maxTry = 10
    cnt = 0
    res = ''
    while(cnt < maxTry):
        try:
            req = requests.get(url=url, headers=headers)
            res = req.text
            break
        except Exception as e:
            cnt += 1
            time.sleep(1)

    html = BeautifulSoup(res, features='html.parser')
    #print(html)
    contents = html.find('div', id = 'js-content', class_ = 'content')
    if contents is None: return content

    ps = contents.find_all('p')
    for p in ps: content += p.text
    #print(content)
 
    return content

def download(keyword, t):
    #print("Inside download: " + keyword)
    global detail_url
    #t = topic()
    t.keyword = keyword

    url = detail_url.replace('XXX', keyword)
    print("URL =" + url)
    req = requests.get(url=url, headers=headers)
    res = req.text
    div_bf = BeautifulSoup(res, features='html.parser')
    #print(div_bf)

    #parse title
    t.title_name = ''
    temp = div_bf.find('div', class_ = 'header-name')
    if(temp): t.title_name = temp.text
    if(t.title_name == ''): return

    t.title_img = ''
    temp =  div_bf.find('img', class_ = 'head-channel-img')
    if(temp): 
        try:
            t.title_img = "https:" + temp['src']
        except Exception as e:
            t.title_img = ''

    idx = 0
    maxIdx = 5 
    three_images_flag = True
    three_images_url = ""
    one_image_flag = True
    one_image_url = ""
    #parse content news
    items = div_bf.find_all('a', class_ = 'piece')
    for item in items:
        idx += 1
        n = news()

        n.href = ""
        try:
            n.href = 'http://www.yidianzixun.com' + item['href']
        except Exception as e:
            n.href = ""

        #check if it's video
        try:
            temp = item['data-id']
            n.type_ = 'video'
            n.videotime = item.find('div', class_ = 'video_time').text
        except Exception as e:
            n.type_ = 'news' 

        try:
            n.title = item.find('div', class_ = 'doc-title').text
            n.title = n.title.replace('\u200b', '')
        except Exception as e:
            n.title_ = '' 

        f = False
        #check if it has three images
        imgs = item.find_all('div', class_ = 'doc-image back-img')
        if(imgs):
            f = True
            for img in imgs:
                try:
                    n.imgs.append('http:' + img['data-bgsrc'])
                except Exception as e:
                    continue
            #setup three_images_url
            if(idx > 0 and idx <= maxIdx and three_images_flag == True and len(n.imgs) > 0):
                three_images_url = n.imgs[0]
                three_images_flag = False

            #setup content
            n.content = downloadContent(n.href) 

        #check if it has only one image
        if(not f):
            img = item.find('div', class_ = 'doc-image-small back-img')
            if(img):
                f = True
                try:
                    n.imgs.append(img['data-bgsrc'])
                except Exception as e: 
                    continue
                #setup one_image_url
                if(idx > 0 and idx <= maxIdx and one_image_flag == True and len(n.imgs) > 0):
                    one_image_url = n.imgs[0]
                    one_image_flag = False

                #setup content
                n.content = downloadContent(n.href) 
 
        #check if it's video background image 
        if(not f):
            img = item.find('div', class_ = 'doc-image-video back-img')
            if(img):
                f = True
                try:
                    n.imgs.append(img['data-bgsrc'])
                except Exception as e:
                    continue

        n.source = "一点资讯"
        n.time = "1小时前"
        temps = item.find_all('span', class_ = 'doc-info-item')
        if(len(temps) == 2):
            #hard code "一点资讯"
            '''
            if temps[0]: 
                try:
                    n.source = temps[0].text
                except Exception as e:
                    continue
            '''
            if temps[1]: 
                try:
                    n.time = temps[1].text
                    n.time.replace('\u00a0', '')
                except Exception as e:
                    continue

        t.news.append(n)

    #setup title image
    if(one_image_flag == False):
        t.title_img = one_image_url
    elif(three_images_flag == False):
        t.title_img = three_images_url

    #logging(t)

    global tempTopic
    tempTopic = t 

    return

def generate(t):
    if(len(t.rules) == 0): 
        print("Rules number is 0, ignore it")
        print("-------------------------------------------------------------------" + "\n")
        return 

    output = {} 

    output["titleName"] = t.title_name
    #print(t.title_name)
    output["titleImage"] = t.title_img
    #print(t.title_img)
    output["keywordList"] = t.rules 

    output["news"] = []
    for n in t.news:
        item = {}
        item["href"] = n.href
        item["type"] = n.type_ 
        if(n.type_ == "video"): 
            item["videoTime"] = n.videotime 
        item["title"] = n.title
        item["content"] = n.content
        #print(n.title)

        images = []
        for img in n.imgs: 
            images.append(img)
        item["images"] = images
 
        item["source"] = n.source
        item["time"] = n.time
        output['news'].append(item)

    result = json.dumps(output)
    #print(result)

    if(not exist(t.title_name)): 
        insert(result)

    return

def exist(title):
    title = title.replace('\"', '\\"')
    detail = '{\"query\": {\"match\": {\"titleName\": \"'+ title +'\"}}, \"_source\": [\"_id\"], \"size\":\"10000\"}'
    res = es.search(index=index, doc_type=dataType, body=detail)
    obj = json.dumps(res)
    obj2 = json.loads(obj)
    cnt = obj2["hits"]["total"]
    if(cnt>0): 
        print(title + " exists in ES, ignore it")
        print("-------------------------------------------------------------------" + "\n")
        return True
    else: 
        print(title + " not exists in ES, insert it")
        return False

def insert(result):
    print(result)
    res = es.index(index=index, doc_type=dataType, body=result, refresh=True)
    updateUI(res["_id"])
    return

def updateUI(id):
    deleteUrl = "http://smart.sanjiaoshou.net/web/activityrecommend/yddel?id=" + id
    f = requests.get(url = deleteUrl)
    print(deleteUrl)
    print("Delete " + id + " from UI.")
    print("-------------------------------------------------------------------" + "\n")

    insertUrl = "http://smart.sanjiaoshou.net/web/activityrecommend/ydset?id=" + id
    f = requests.get(url = insertUrl)
    print(insertUrl)
    print("Insert " + id + " into UI.")
    print("-------------------------------------------------------------------" + "\n")

def find():

    global res
    global topics

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
        s1 = ''
        content = []

        i.title =topic.keyword 
        s += topic.keyword + '\n'
        content.append(topic.keyword)

        for n in topic.news:
            s1 += "Title: " + n.title + '\n'
            s1 += "Content: " + n.content + '\n\n'
            s += n.title + '\n'
            #s += n.content + '\n\n'
            content.append(n.title)

        i.num = num
        i.text = s
        i.print = s1
        i.content = content
        i.freq_num = freq
        i.tfidf_num = tfidf
        i.textRank_num = textRank
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

    idx = 0
    for r in res:
        r.oneKeywordCandidate = merge(r.union, r.content)
        r.oneKeywordRecommand, r.twoKeywordsCandidate = checkOneKeyword(r.oneKeywordCandidate) 
        r.twoKeywordsRecommand, r.threeKeywordsCandidate = checkTwoKeywords(r.twoKeywordsCandidate, r.oneKeywordCandidate) 
        r.threeKeywordsRecommand, r.discardKeywords = checkThreeKeywords(r.threeKeywordsCandidate, r.oneKeywordCandidate) 

        print("<<专题名称>>" + "\n" + r.title + "\n")
        print("<<新闻列表>>" + "\n" + r.print + "\n")
        print("Freq = " + str(r.freq_num) + "\t" + str(r.freq))
        print("Tfidf = " + str(r.tfidf_num) + "\t" + str(r.tfidf))
        print("TextRank = " + str(r.textRank_num) + "\t" + str(r.textRank))
        print("Union" + "\t" + str(r.union))
        print("\n")
        print("One Keyword Candidate" + "\t" + str(r.oneKeywordCandidate))
        print("One Keyword Recommand")
        for k in r.oneKeywordRecommand:
            print(log[k])
        print("\n")
        print("Two Keywords Candidate" + "\t" + str(r.twoKeywordsCandidate))
        print("Two Keywords Recommand")
        for k in r.twoKeywordsRecommand:
            tt = "%20".join(k)
            print(log[tt])
        print("\n")
        print("Three Keywords Candidate" + "\t" + str(r.threeKeywordsCandidate))
        print("Three Keywords Recommand")
        for k in r.threeKeywordsRecommand:
            tt = "%20".join(k)
            print(log[tt])
        print("\n")
        print("Discard Keywords" + "\t" + str(r.discardKeywords))
        print("\n")
        tt = ""
        if(len(r.oneKeywordRecommand) > 0): tt += str(r.oneKeywordRecommand)
        if(len(r.twoKeywordsRecommand) > 0): tt += str(r.twoKeywordsRecommand)
        if(len(r.threeKeywordsRecommand) > 0): tt += str(r.threeKeywordsRecommand)
        print("Final Recommandation" + "\t" + tt) 
        print("\n")

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
    global log

    threshold_1 = 0.5
    threshold_2 = 0.5

    candidate = []
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
            log[t] = info
            tt = []
            tt.append(keywords[i][0])
            tt.append(keywords[i][1])
            tt.append(keywords[i][2])
            recommand.append(tt)  
        else:
            candidate.append(keywords[i])

    return recommand, candidate

def checkTwoKeywords(keywords, candidates):
    global log

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
                log[t] = info
                tt = []
                tt.append(keywords[i])
                tt.append(keywords[j])
                recommand.append(tt)  
            else:
                for k in range(len(keywords)):
                    if(k == i or k == j): continue
                    c = []
                    c.append(keywords[i])
                    c.append(keywords[j])
                    c.append(keywords[k])
                    c.sort()
                    t_c.append(c)

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
            log[keywords[i]] = info
            flag[i] = 1
            recommand.append(keywords[i])  

    candidate = []
    for i in range(len(flag)):
        if(flag[i] == 0): 
            candidate.append(keywords[i])

    return recommand, candidate

def search(keyword):
    rs = []
    u = url.replace('XXX', urllib.parse.quote(keyword))
    req = requests.get(url=u, headers=chrome_headers)
    res = req.content
    #print(res)
    div_bf = BeautifulSoup(res, features='html.parser')
    items = div_bf.find_all('div', class_ = 'result c-container')
    for item in items:
        title = item.find('h3').text
        rs.append(title)
    return rs 

if __name__ == '__main__':

    #download topics
    t = str(int(time.mktime(time.localtime())))
    news_url = news_url.replace("XXX", t)
    req = requests.get(url = news_url)
    res = req.text
    content = json.loads(res)
    keywords = content["keywords"]
    for keyword in keywords: print(keyword)
    maxTry = 10
    for keyword in keywords:
        k = keyword["name"]
        t = topic()
        for cnt in range(maxTry):
            print("Start download " + k)
            download(k, t)
            if(len(t.news) == 0):
                time.sleep(2)
                continue
            else:
                topics.append(t)
                time.sleep(2)
                break

    #find keywords
    find()

    sys.exit()

