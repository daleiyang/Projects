# -*- coding: utf-8 -*-
import json
import sys

#reload(sys)
#sys.setdefaultencoding('utf8')

from elasticsearch import Elasticsearch
es = Elasticsearch("http://172.16.0.213:28405")
index = "highlight_news"
dataType = "external"

def delete():
    res = es.delete(index=index, doc_type=dataType, id="name￥ﾷﾲ￥ﾭﾘ￥ﾜﾨ")
    print("delete done.")
    return

if __name__ == '__main__':
    delete()

