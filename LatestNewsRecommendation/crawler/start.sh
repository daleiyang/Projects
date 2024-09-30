#!/bin/bash

datetime=`date +%Y%m%d%H%M%S`
pid=$(cat server.pid)

if [ ! -n "$pid" ]; then
  echo "No process running"
else
  echo "Previous crawler is running, kill it."
  echo "kill $pid"
  kill -9 $pid
  cat /dev/null > /home/trio/yangdalei/crawler/server.pid
fi
nohup /home/trio/yangdalei/anaconda3/bin/python3.7 /home/trio/yangdalei/crawler/crawler.py > /home/trio/yangdalei/crawler/crawler.log 2>&1 &
echo $!>/home/trio/yangdalei/crawler/server.pid
echo "Crawler is running." 

