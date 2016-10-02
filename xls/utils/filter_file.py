#!/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os
import xlrd
from xlutils import copy
import hashlib

reload(sys)
sys.setdefaultencoding('utf8')

workdir=os.getcwd()


#######################
#
#过滤文件：
#	1、文件中的所有表单均不符合要求
#	2、文件中有部分表单符合要求
#	3、文件中的所有表单符合要求
#	将第1种文件移动到某个目录，第2种文件只处理符合条件的表单
#
##############

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

	__dict=[row_dict, info_dict]
	return __dict


#验证一个表单是否符合要求，不符合返回-1,符合返回>=0的值
#参数	
#	booksheet	sheet表单对象
#	dictbook  	标题行关键字列表
#	maxline		最多取maxline行来验证一个表单是否符合要求
def TLine(booksheet, dictbook, maxline=5):
	
	flist=[]
	line=booksheet.nrows
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
		print strs	

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


#如果文件的全部表单都符合要求，则不对该文件进行分离操作;
#否则在遇到该文件中的第一个不符合的表单的时候，停止识别，
#从该文件第一个表单开始重新识别，然后将符合条件的表单分离
def filter_file(fname='test.xls',\
		r_dict=['NODICT', 'NODICT'],\
		i_dict=['NODICT', 'NODICT']):
	
	current_dir=workdir+"/test/"

	book=xlrd.open_workbook(current_dir+fname)
	print "There are {} sheets in the book.".format(book.nsheets)

	for sheet in book.sheets():
		Flag=False
		if TLine(sheet, r_dict)>=0:#表单符合,继续下一个表单
			continue
		else:
			Flag=True
			break
	if Flag:#如果该文件有不符合条件的表单
		for sheet in book.sheets():
			if TLine(sheet, r_dict)>=0:#分离符合条件的表单
				new_book=copy.copy(book)
				new_book._Workbook__sheets=\
					[new_book._Workbook__worksheets[0]]
				new_book.save(\
					workdir+"/xls/"+\
					hashlib.sha1(fname).hexdigest()+\
					'_{}.xls'.format(sheet.name))
		os.system('mv '+current_dir+fname+' '+workdir+'/nomatch/')
	else:
		os.system('mv '+current_dir+fname+' '+workdir+'/xls/')
		

def main():
	_dict = _load_dict()

	cur_dir=workdir+"/test/"
	
	flist=os.listdir(cur_dir)
	for filename in xrange(len(flist)):
		print
		if os.path.isfile(cur_dir+flist[filename]):
			print "文件名："+flist[filename]
			filter_file(flist[filename], _dict[0], _dict[1])
		else:
			print flist[filename]+"  是目录"


main()
