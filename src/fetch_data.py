#!/usr/bin/python

import json, urllib, sys, Queue
from multiprocessing.pool import ThreadPool as Pool

queue = Queue.Queue()

def getJson(line):
   u = urllib.urlopen(line)
   queue.put(json.load(u))
   u.close()

queries = None

with open(sys.argv[1], 'r') as f:
   queries = [line for line in f]

pool = Pool()

pool.map(getJson, queries)

while not queue.empty():
   obj = queue.get()
   for i in obj['query']['results']['rate']:
      print i['id'][:3], i['id'][3:], i['Rate']
