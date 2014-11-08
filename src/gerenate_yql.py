#!/usr/bin/python

import json, math, multiprocessing, os.path, sys

script_dir = os.path.realpath(os.path.dirname(__file__)) + '/'
queries = []
pairs = []

if sys.argv[1] == 'p':
   file_name = 'currency_pairs.txt'
   with open(script_dir + file_name, 'r') as f:
      for line in f:
         pairs.append(line.strip())

elif sys.argv[1] == 'c' or sys.argv[1] == 'a':
   file_name = 'currencies.json' if sys.argv[1] == 'c' else 'currencies.all.json'
   l = [ x for x in json.load(open(script_dir + file_name, 'r')).iterkeys() ]
   n = len(l)
   pairs = [ "\"%s%s\"" % (l[i], l[j]) for i in xrange(1, n) for j in xrange(i) ]

m = len(pairs)

limit = int(math.ceil(m/math.ceil(m/200.0))) if m/200.0 > multiprocessing.cpu_count() else int(math.floor(float(m) / multiprocessing.cpu_count()))

for i in xrange(0, m, limit):
   queries.append( "http://query.yahooapis.com/v1/public/yql?q=select%20%2a%20from%20yahoo.finance.xchange%20where%20pair%20in%20%28" \
         + ','.join(pairs[i:i+limit]) \
         + "%29&env=store://datatables.org/alltableswithkeys&format=json" )

print '\n'.join(queries)
