#-*- coding:utf-8 -*-

import sys,urllib2,time
import gzip,StringIO
import re,random
from bs4 import BeautifulSoup

import xlrd,xlwt
from xlutils.copy import copy

reload(sys)
sys.setdefaultencoding('utf8')

class SEResult:
	

	def __init__(self):
	
		#配置默认值,变量都是以两个下划线开头
		__agent_file	= './conf/user_agents'
		__re_pattern	= '.*?\\\"(http://.*?\\.swu\\.edu\\.cn.*?)\\\".*?<strong>.*'
		__xls_file	= './conf/url.xls'
		__position_file	= './conf/pos.cfg'
		__default_url	= 'http://www.bing.com/search?q=site%3Aswu.edu.cn+filetype%3Axls&first='
		__default_step	= 10
		__default_start	= 1
			

		#载入头部的用户代理
		self.user_agent=[]
		fp 	= open(__agent_file, 'r')
		line 	= fp.readline().strip('\n')
		while(line):
			self.user_agent.append(line)
			line 	= fp.readline().strip('\n')
		fp.close()

		#打开xls文件
		_urlbook	= xlrd.open_workbook(__xls_file)
		index		= _urlbook.nsheets
		self.row_pos	= _urlbook.sheets()[index-1].nrows	#写入表单的行数
		self.urlbook	= copy(_urlbook)
		self.urlsheet	= self.urlbook.get_sheet(index-1)	#打开最后一个表单

		self.pattern = re.compile(r'{}'.format(__re_pattern))
		
		#设置总的页面数，请求页面开始/结束位置，初始请求链接
		
		#从文件获取请求页面的位置
		fs = open(__position_file)
		self.page	= int(fs.read())
		fs.close()

		self.s_url	= __default_url
		self.step	= __default_step
		self.start	= __default_start
		self.xls_file	= __xls_file
		self.position_file	= __position_file

	def openUrl(self):
		
		#构建请求url
		p	= self.page
		step	= self.step
		start	= self.start
		
		p	= p*step+start
		l	= [self.s_url, '%d'%p]
		url	= ''.join(l)


		request = urllib2.Request(url)
		length	= len(self.user_agent)
		index	= random.randint(0,length-1)
		request.add_header('User-agent', self.user_agent[index])
		#request.add_header('Accept-Encoding', 'gzip,deflate')
		request.add_header('referer', 'www.google.com.hk')
		request.add_header('Accept-Language', 'zh-cn')
		request.add_header('Connection', 'keep-alive')
		
		response= urllib2.urlopen(request)
		
		html 	= response.read()
		if response.headers.get('content-encoding',None)=='gzip':
			html	= gzip.GzipFile(fileobj=StringIO.StringIO(html)).read()
		
		soup	= BeautifulSoup(html)
		self.sl	= soup.select('a')
		
		self.page	= self.page+1
		

	def extractUrl(self):
		
		#解析页面,提取url
		sl	= self.sl
		pattern = self.pattern
		urllist	= []
		
		if len(sl) <=0:
			print 'sl =0'

		for x in xrange(len(sl)):
			s = '{}'.format(sl[x])
			if pattern.match(s):
				tmps	= pattern.sub(r'\1', s)
				urllist.append(tmps)
		
		#如果请求的页面无法提取有效url，说明服务器拒绝该请求，返回的是验证页面
		#因此触发错误，以便停止请求一段随机的时间
		x	= len(urllist)
		if x<=0:
			self.page	= self.page-1
			self.sl	= []
			raise NotImplementedError 

		#将url写入xls文件保存,逐行写入
		row	= self.row_pos
		for col in xrange(x):
			_s	= urllist[col].decode('utf-8')
			self.urlsheet.write(row, col, _s)
			print _s
		self.row_pos	= self.row_pos+1
	
	def save(self):

		#保存当前请求的页面位置、保存xls文件，然后退出
		xlsSavePath	= self.xls_file
		posSavePath	=self.position_file	

		fs = open(posSavePath, 'w')
		fs.write(str(self.page))
		fs.close()
		
		self.urlbook.save(xlsSavePath)

	def sleepMoment(self):
		t	= random.randint(60,90)
		print 'sleep %d seconds'%t
		time.sleep(t)

inst	= SEResult()

while 1:
	retry = 3
	while retry > 0:
		try:
			inst.openUrl()
		except KeyboardInterrupt:
			inst.save()
			sys.exit(1)
		except:
			retry = retry-1
		else:
			break
	
	try:
		inst.extractUrl()
		inst.save()
		#inst.sleepMoment()
	except NotImplementedError:
		inst.save()
		print 'server denied'
		sys.exit(1)
	except:
		inst.save()
		print 'error occur'
		sys.exit(1)
