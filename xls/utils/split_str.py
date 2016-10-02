#!/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os

reload(sys)
sys.setdefaultencoding('utf8')

fh=open("rowdict")
lines=""

for line in fh.readlines():
	lines = lines+line
print lines
dictbook=lines.split(";")
for i in xrange(len(dictbook)):
	print dictbook[i]
