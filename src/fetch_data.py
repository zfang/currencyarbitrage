#!/usr/bin/python

import json, urllib, Queue, fileinput
from multiprocessing.pool import ThreadPool as Pool

pool = Pool()

def getJson(line):
   u = urllib.urlopen(line)
   item = json.load(u)
   u.close()
   return item

results = [ pool.apply_async(getJson, [line]) for line in fileinput.input() ]

for result in results:
   obj = result.get()
   for i in obj['query']['results']['rate']:
      print i['id'][:3], i['id'][3:], i['Ask'], i['Bid']
