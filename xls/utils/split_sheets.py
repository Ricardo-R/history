#!/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os
import xlrd
from xlutils import copy

reload(sys)
sys.setdefaultencoding('utf8')


#验证一个表单是否符合要求，不符合返回-1,符合返回>=0的值
def TLine(booksheet, maxline=5):

#载入标题行识别关键字元组
	fh=open("rowdict", "r")
	lines=""

	for line in fh.readlines():
		lines = lines+line
	dictbook=lines.split(";")
	fh.close()

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
def filter_file(file='test.xls'):
	book=xlrd.open_workbook(file)
	print "There are {} sheets in the book.".format(book.nsheets)

	for sheet in book.sheets():
		Flag=False
		if TLine(sheet)>=0:#表单符合,继续下一个表单
			continue
		else:
			Flag=True
			break

	if Flag:#该文件有不符合条件的表单
		for sheet in book.sheets():
			if TLine(sheet) > 0:#分离符合条件的表单
				new_book=copy.copy(book)
				new_book._Workbook__sheets=\
					[new_book._Workbook__worksheets[0]]
				new_book.save('{}_.xls'.format(sheet.name))
				print "Yes,",


def main():
	if len(sys.argv)==2:
		dir_name=sys.argv[1]
	else:
		dir_name=os.getcwd()+"/xls/";
	flist=os.listdir(dir_name)

	for filename in range(len(flist)):
		print
		if os.path.isfile(dir_name+flist[filename]):
			print "文件名："+flist[filename]
			filter_file(dir_name+flist[filename])
		else:
			print flist[filename]+"  是目录"


main()
