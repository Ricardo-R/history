#/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os

reload(sys)
sys.setdefaultencoding('utf8')

class conf:
	def __init__(self):

		self.defaultDict= {}
		lines = []
		
		#读取默认配置文件的内容
		if not os.path.isfile('./conf/default.conf'):
			print 'default.conf missing'
			exit(1)
		fs = open('./conf/default.conf', 'r')
		line = fs.readline()
		while(line):
			if not(line.startswith('#') or line.startswith(' ') or line.startswith('\n')):
				lines.append(line.strip('\n'))
			line = fs.readline()

		#处理默认配置文件的内容，构建字典
		for i in xrange(len(lines)):
			keyValue = lines[i].split()
			self.defaultDict[keyValue[0]]=keyValue[1]

	def app(self):
	
		lines = []
		if not os.path.isfile('./conf/user.conf'):
			print 'user.conf missing'
			exit(1)

		fs = open('./conf/user.conf', 'r')
		line = fs.readline()
		while(line):
			if not(line.startswith('#') or line.startswith(' ') or line.startswith('\n')):
				lines.append(line.strip('\n'))
			line = fs.readline()
	
		for i in xrange(len(lines)):
			key = lines[i].split()
			if self.defaultDict.has_key(key[0]):
				self.defaultDict[key[0]]=key[1]

if __name__=='__main__':
	s = conf()
	s.app()
