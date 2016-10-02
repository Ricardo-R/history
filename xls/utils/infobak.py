#!/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os
import xlrd
from xlutils.copy import copy
import hashlib
import string

reload(sys)
sys.setdefaultencoding('utf8')

workdir=os.getcwd()


#载入字典
def _load_dict():
	
	#载入标题行识别字典
	fs=open(workdir+"/dict/rowdict", "r")
	lines=""
	for line in fs.readlines():
		lines = lines+line
	row_dict = lines.split(";")
	fs.close()
	print "Loading dictionary complete . . . "

	#载入列标题识别字典
	fs = open(workdir+"/dict/infodict", "r")
	info_dict={}
	for line in fs.readlines():
		cell	= line.split(';')
		col_key	= cell[0]
		col_val	= cell[1]
		info_dict[col_key] = col_val
	fs.close()

	#载入写入表单字典
	fs = open(workdir+"/dict/wrdict", "r")
	wr_dict={}
	for line in fs.readlines():
		cell	= line.split(';')
		col_key	= cell[0]
		col_val	= string.atoi(cell[1])
		wr_dict[col_key] = col_val
	fs.close()

	__dict=[row_dict, info_dict, wr_dict]
	return __dict


#验证一个表单是否符合要求，不符合返回-1,符合返回行号(>=0)
#参数	
#	booksheet	sheet表单对象
#	dictbook  	标题行关键字列表
#	maxline		最多取maxline行来验证一个表单是否符合要求
def TLine(booksheet, dictbook, maxline=5):
	
	flist=[]
	line=booksheet.nrows
	#空表单
	if line == 0:
		return -1
	#最多取表单的前maxline行来进行标题行的识别
	if line > maxline:
		for row in xrange(maxline):
			flist.append(booksheet.row_values(row))
	else:
		for row in xrange(line):
			flist.append(booksheet.row_values(row))

	#拼接各行的列值，然后进行验证是否符合标题行的要求
	for row in xrange(len(flist)):
		strs=""
		s=""
		
		for col in xrange(len(flist[row])):#拼接一行的各列
			if isinstance(flist[row][col], float):
				s=str(flist[row][col])
			elif isinstance(flist[row][col], int):
				s=str(flist[row][col])
			else:
				strs=strs+flist[row][col]
			strs=strs+s

		#将拼接好的字符串验证
		if strs=="":
			continue
		else:
			if strs.find(dictbook[0])<0:
				continue
			for i in xrange(1,len(dictbook)):
				if strs.find(dictbook[i])>=0:
					return row
	return -1


#处理单个表单：
#	构建源文件字典，开始处理表单列
#	根据写入表单字典，向新表单列中写入数据
def single_sheet(sheet, row, info_dict, wr_dict, info_sheet, appendrow):
	
	#表单只有标题行的情况
	if sheet.nrows == (row+1):
		return
	
	#构建源文件字典
	src_dict={}
	flist=[]
	flist.append(sheet.row_values(row))
	for col in xrange(len(flist[0])):
		s=sheet.cell_value(row,col).strip()
		title='fail'
		for key in info_dict:
			if info_dict[key].find(s)>=0:#在字典中识别出来
				title=key
				break
		#识别成功,将列号和列标题加入源文件字典
		if title!='fail':
			src_dict[col] = title
	
	#复制信息，写入表单

	#由于是按列写入，因此处理一个表单时，追加的行的位置相同
	#所以行开始追加的行号被放在循环外边
	for col in xrange(len(flist[0])):
		if src_dict.has_key(col):
			
			#根据识别的列，得到对应写入的列号
			info_sheet_col = wr_dict[src_dict[col]]
			
			#每次最多读取该列的500行数据，然后写入
			buf_list=[]
			for left in xrange(row+1, sheet.nrows):
				buf_list.append(\
					sheet.cell_value(left, col))
			print "len(buf_list):%d"%len(buf_list)
			for inrow in xrange(len(buf_list)):
				info_sheet.write(appendrow+inrow,\
						info_sheet_col,\
						buf_list[inrow])



#过滤文件是过滤文件部分的功能，这里处理过滤后符合条件的文件，
#如果有文件不符合条件的，直接跳过该文件，继续下一个文件的处理
def deal_single_file(fname='test.xls',\
		r_dict=['NODICT', 'NODICT'],\
		i_dict={}, wr_dict={}):
	
	infobook = xlrd.open_workbook(workdir+"/info/info.xls",\
					formatting_info=True)
	_infobook= copy(infobook)
	infosheet= _infobook.get_sheet(0)

	current_dir=workdir+"/xls/"

	book=xlrd.open_workbook(current_dir+fname)
	print "There are {} sheets in the book.".format(book.nsheets)

	for sheet in book.sheets():
		rlist=[]
		Flag=False
		row = TLine(sheet, r_dict)
		if row < 0:#表单不符合,忽略之,继续下一个表单
			continue
		else:
			#处理这个符合条件的表单
			single_sheet(sheet, row, i_dict, wr_dict,\
					infosheet,\
					infobook.sheet_by_index(0).nrows)
		
	_infobook.save(workdir+"/info/info.xls")

def info_file():
	_dict = _load_dict()

	cur_dir=workdir+"/xls/"

	
	flist=os.listdir(cur_dir)
	for filename in xrange(len(flist)):
		print
		if os.path.isfile(cur_dir+flist[filename]):
			print "正在处理文件："+flist[filename]
			deal_single_file(flist[filename], _dict[0],\
					_dict[1], _dict[2])
		else:
			print flist[filename]+"  是目录"


info_file()
