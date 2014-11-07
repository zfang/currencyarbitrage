#!/usr/bin/python

import json, math, multiprocessing

l = [ x for x in json.load(open('currencies.json', 'r')).iterkeys() ]

n = len(l)

pairs = [ "\"%s%s\"" % (l[i], l[j]) for i in xrange(1, n) for j in xrange(i) ]

m = len(pairs)

limit = int(math.ceil(m/math.ceil(m/200.0))) if m/200.0 > multiprocessing.cpu_count() else int(math.ceil(float(m) / multiprocessing.cpu_count()))
#limit = int(math.ceil(m/math.ceil(m/200.0)))
queries = []

for i in xrange(0, m, limit):
   queries.append( "http://query.yahooapis.com/v1/public/yql?q=select%20%2a%20from%20yahoo.finance.xchange%20where%20pair%20in%20%28" \
         + ','.join(pairs[i:i+limit]) \
         + "%29&env=store://datatables.org/alltableswithkeys&format=json" )

print '\n'.join(queries)
