#!/usr/bin/python
#-*- coding:utf-8 -*-

import sys, os
import xlrd
from xlutils.copy import copy
import xlwt
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

		#要符合标题行的条件，姓名+身份证或姓名+学号/工资号/职工号
		#是必须的
		if strs=="":
			continue
		else:
			#姓名一列是必须的
			if strs.find(dictbook[0])<0:
				continue
			if strs.find(dictbook[1]) >= 0:
				return row
			elif strs.find(dictbook[2]) >= 0:
				return row
			elif strs.find(dictbook[3]) >= 0:
				return row
	return -1


#将传入的表单单独保存为一个excel文件
def split_sheet(sheet, save_path):
	new_book = xlwt.Workbook()	
	booksheet= new_book.add_sheet("sheet 1", cell_overwrite_ok=True)
	
	for row in xrange(sheet.nrows):
		for col in xrange(sheet.ncols):
			booksheet.write(row, col,\
					sheet.cell_value(row, col))
	new_book.save(save_path)

#如果文件的全部表单都符合要求，则不对该文件进行分离操作;
#否则在遇到该文件中的第一个不符合的表单的时候，停止识别，
#从该文件第一个表单开始重新识别，然后将符合条件的表单分离
def filter_single_file(fname='test.xls',\
		r_dict=['NODICT', 'NODICT']):
	
	current_dir=workdir+"/source/"

	book=xlrd.open_workbook(current_dir+fname)
	print ", {}个表单".format(book.nsheets)

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
				split_sheet(sheet, workdir+"/xls/"+\
					hashlib.sha1(fname).hexdigest()+\
					'_{}.xls'.format(sheet.name))
		
		os.system('mv '+current_dir+fname+' '+workdir+'/nomatch/')
	else:
		os.system('mv '+current_dir+fname+' '+workdir+'/xls/')
		
#过滤文件
def filter_file(_dict):

	cur_dir=workdir+"/source/"
	
	flist=os.listdir(cur_dir)
	for filename in xrange(len(flist)):
		print
		if os.path.isfile(cur_dir+flist[filename]):
			print "正在过滤文件："+flist[filename],
			filter_single_file(flist[filename], _dict[0])
		else:
			print "略过目录："+flist[filename]

#处理单个表单：
#	构建源文件字典，开始处理表单列
#	根据写入表单字典，向新表单列中写入数据
def info_single_sheet(sheet, title_row, info_dict, wr_dict,\
		info_sheet, appendrow):
	
	#表单只有标题行的情况
	if sheet.nrows == (title_row+1):
		return
	
	#构建源文件字典
	src_dict={}
	flist=[]#标题行
	flist.append(sheet.row_values(title_row))
	for col in xrange(len(flist[0])):
		s=sheet.cell_value(title_row,col).strip()
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
	
	#计算外围控制循环次数times
	n_rows = 50	#每次读取的数据
	k = n_rows-1
	x = sheet.nrows-(title_row+1)	#总共要处理的行数
	times = (x+k)/n_rows

	for col in xrange(len(flist[0])):
		if src_dict.has_key(col):
			
			#根据识别的列，得到对应写入的列号
			info_sheet_col = wr_dict[src_dict[col]]
			
			#每次处理表单中新的一列时，恢复读取和写入位置
			rd_start_p	= title_row+1	#title_row是标题行号
			rd_end_p	= title_row+1
			wr_start_p	= appendrow
			
			#外循环,控制写完一列的数据
			for t in xrange(times):
				#改变终点
				rd_end_p	= rd_end_p + n_rows
				if rd_end_p > sheet.nrows:
					rd_end_p = sheet.nrows
			
				#读取数据
				buf_list=[]
				for row in xrange(rd_start_p, rd_end_p):
					buf_list.append(\
						sheet.cell_value(row, col))
					
				#写入数据
				wr_rows = len(buf_list)#每一次写入的行数
				for row in xrange(wr_rows):
					info_sheet.write(wr_start_p+row,\
							info_sheet_col,\
							buf_list[row])
				
				#每写入wr_rows行的数据后改变写入数据的起点
				wr_start_p = wr_start_p + wr_rows

				#每处理完n_rows再改变读数据的起点
				rd_start_p = rd_start_p + n_rows
		
		#列标题不能被识别的列的不做处理


#过滤文件是过滤文件部分的功能，这里处理过滤后符合条件的文件，
#如果有表单不符合条件的，直接跳过继续下一个表单的处理
def info_single_file(fname='test.xls',\
		r_dict=['NODICT', 'NODICT'],\
		i_dict={}, wr_dict={}):
	
	infobook = xlrd.open_workbook(workdir+"/info/info.xls",\
					formatting_info=True)
	_infobook= copy(infobook)
	infosheet= _infobook.get_sheet(0)

	current_dir=workdir+"/xls/"

	book=xlrd.open_workbook(current_dir+fname)
	print "共{}个表单".format(book.nsheets)

	for sheet in book.sheets():
		rlist=[]
		Flag=False
		row = TLine(sheet, r_dict)
		if row < 0:#表单不符合,忽略之,继续下一个表单
			continue
		else:
			#处理这个符合条件的表单
			info_single_sheet(sheet, row, i_dict, wr_dict,\
					infosheet,\
					infobook.sheet_by_index(0).nrows)
		
	_infobook.save(workdir+"/info/info.xls")
	os.system('mv '+current_dir+fname+' '+workdir+'/workdone/')

def info_file(_dict):
	
	cur_dir=workdir+"/xls/"
	
	flist=os.listdir(cur_dir)
	for filename in xrange(len(flist)):
		print
		if os.path.isfile(cur_dir+flist[filename]):
			print "正在处理文件："+flist[filename]
			info_single_file(flist[filename], _dict[0],\
					_dict[1], _dict[2])
		else:
			print "略过目录："+flist[filename]

def main():
	_dict = _load_dict()
	
	filter_file(_dict)

	info_file(_dict)

main()
