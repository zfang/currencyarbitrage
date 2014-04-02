#!/usr/bin/python

import json

l = [ x for x in json.load(open('currencies.json', 'r')).iterkeys() ]

n = len(l)

pairs = []

for i in xrange(1, n):
   for j in xrange(i):
      pairs.append("\"%s%s\"" % (l[i], l[j]))

m = len(pairs)

limit = 200
queries = []

for i in xrange(0, m, limit):
   queries.append( "http://query.yahooapis.com/v1/public/yql?q=select%20%2a%20from%20yahoo.finance.xchange%20where%20pair%20in%20%28" \
         + ','.join(pairs[i:i+limit]) \
         + "%29&env=store://datatables.org/alltableswithkeys&format=json" )

print '\n'.join(queries)
