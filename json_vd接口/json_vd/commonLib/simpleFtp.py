#!/bin/env python
# -*- coding: UTF-8 -*-

__version__='1.0.0.0'
__author__='jiweichao@baidu.com'
__date__='2012-11-14'

import os
import socket
import ftplib

from ftplib import FTP

class simpleFtp:
	'''
	@summary:
	'''
	def __init__(self, host, port='21', username='work', passwd='ps-testing!!!'):
		self.__host = host
		self.__port = port
		self.__username = username
		self.__passwd = passwd
		self.__file_list = []
		self.__folder_list = []


	def __upload_file(self, localfile, remotefile, FTPInstance):
		'''
		@summary: 
				upload single file to remote host
				
		'''
		if not os.path.isfile(localfile):
			print '%s is not a file' % localfile
			return -1 
	
		file_handler = open(localfile,'rb')
		try:
			FTPInstance.storbinary('STOR %s' % remotefile, \
									file_handler)
		except ftplib.error_perm:
			print '[FATAL]Can not read file "%s"' % remotefile
			return -1
		finally:
			file_handler.close()
	
		#print '[INFO]Upload file [%s] success' % localfile
		return 0

	def __download_file(self, localfile, remotefile, FTPInstance):
		'''
		@summary:
				download single file to local host
		'''
		file_handler = open(localfile, 'wb').write
		try:
			FTPInstance.retrbinary('RETR %s' % remotefile, \
									file_handler)
		except ftplib.error_perm:
			print '[FATAL]Can not read file "%s"' % remotefile
			os.unlink(remotefile)
			return -1

		#print '[INFO]Download file [%s] success' % localfile
	def upload_dir_new(self,localdir,remotedir,dirname,create_dir='true'):
		if(create_dir=='true'):
			cloneFTP = FTP()
			try:
				cloneFTP.connect(self.__host, self.__port)
			except (socket.error, socket.gaierror), e:
				print '{\"status\":false,\"info\":\"[FATAL]Can not reach "%s":"%s"\"}' % (self.__host, self.__port)
				return -1
			#print '[DEBUG] connect host well'

			#login
			try:
				cloneFTP.login(self.__username, self.__passwd)
			except ftplib.error_perm:
				print '{\"status\":false,\"info\":\"[FATAL] Ftp login fail\"}'
				cloneFTP.quit()
				return -1
			#print '[DEBUG] login well'
			cloneFTP.cwd(remotedir)
			try:
				cloneFTP.mkd(dirname)
			except:
				print '{\"status\":false,\"info\":\"[FATAL] JobTag already exists\"}'
				return -1
			remote_src = os.path.join(remotedir, dirname)
			self.upload_dir(localdir,remote_src)
			pass

	def upload_dir(self,localdir,remotedir):
		cloneFTP = FTP()
		try:
			cloneFTP.connect(self.__host, self.__port)
		except (socket.error, socket.gaierror), e:
			print '[FATAL]Can not reach "%s":"%s"' % (self.__host, self.__port)
			return -1
		#print '[DEBUG] connect host well'

		#login
		try:
			cloneFTP.login(self.__username, self.__passwd)
		except ftplib.error_perm:
			print '[FATAL] Ftp login fail'
			cloneFTP.quit()
			return -1
		#print '[DEBUG] login well'
        	if not os.path.isdir(localdir):    
            		print 'localdir is no exist!'
			return -1
        	cloneFTP.cwd(remotedir)   
        	for file in os.listdir(localdir):  
            		src = os.path.join(localdir, file)  
            		if os.path.isfile(src):  
                		self.upload_file(localdir,remotedir, file)  
            		elif os.path.isdir(src):  
                		try:    
                    			cloneFTP.mkd(file)    
                		except:    
                    			sys.stderr.write('the dir is exists %s'%file)  
                		self.upload_dir(src, os.path.join(remotedir, file))  
        	cloneFTP.cwd('..')  
	def upload_file(self, localdir, remotedir, file=''):
		'''
		@summary:
				upload single or multi files to remote host
		'''
		#input check
		if not os.path.isdir(localdir):
			print '[FATAL] localdir is not a folder'
			return -1

		cloneFTP = FTP()
		#connect
		try:
			cloneFTP.connect(self.__host, self.__port)
		except (socket.error, socket.gaierror), e:
			print '[FATAL]Can not reach "%s":"%s"' % (self.__host, self.__port)
			return -1
		#print '[DEBUG] connect host well'

		#login
		try:
			cloneFTP.login(self.__username, self.__passwd)
		except ftplib.error_perm:
			print '[FATAL] Ftp login fail'
			cloneFTP.quit()
			return -1
		#print '[DEBUG] login well'
		
		#set current work path
		#if remote dir is not exist, create it
		try:
			cloneFTP.cwd(remotedir)
		except ftplib.error_perm:
			#print '[DEBUG] remote dir is not exist, create it'
			cloneFTP.mkd(remotedir)
			try:
				cloneFTP.cwd(remotedir)
			except ftplib.error_perm:
				print '[FATAL] create fail, dir not exist'
				cloneFTP.quit()	
				return -1
		#print '[DEBUG]CWD %s well' % remotedir

		# Get filelist to upload
		if file == '':
			# upload all file in localdir
			elementlist = os.listdir(localdir)
			#print '[DEBUG] filelis is ' + str(elementlist)
			if elementlist == []:
				print 'Dir is empty, done'
				cloneFTP.quit()
				return -1			
			else:
				for ele in elementlist:
					ele_path = os.path.join(localdir, ele)
					remote_path = os.path.join(remotedir, ele)
					if os.path.isdir(ele_path):	# folder proccess
						if self.upload_file(ele_path, remote_path) == -1:
							print '[FATAL] upload foler<%s> fail ' % ele_path
							cloneFTP.quit()
							return -1
					else:
						if self.__upload_file(ele_path, remote_path, cloneFTP) == -1:
							print '[FATAL] upload file<%s> fail' % ele_path
							cloneFTP.quit()
							return -1
		else:
			remote_file = os.path.join(remotedir, file)
			local_file = os.path.join(localdir, file)
			if self.__upload_file(local_file, remote_file, cloneFTP) == -1:
				print '[FATAL] upload file<%s> fail' % local_file
				cloneFTP.quit()
				return -1
		
		cloneFTP.quit()
		return 0

	def download_file(self, localdir, remotedir, file=''):
		'''
		@summary:
				download single or multi files to local
		'''
		self.__file_list = []
		self.__folder_list = []
		#input check
		if not os.path.isdir(localdir):
			os.mkdir(localdir)

		cloneFTP = FTP()
		#connect
		try:
			cloneFTP.connect(self.__host, self.__port)
		except (socket.error, socket.gaierror), e:
			print '[FATAL]Can not reach "%s":"%s"' % (self.__host, self.__port)
			return -1
		#print '[DEBUG] connect host well'

		#login
		try:
			cloneFTP.login(self.__username, self.__passwd)
		except ftplib.error_perm:
			print '[FATAL] Ftp login fail'
			cloneFTP.quit()
			return -1
		#print '[DEBUG] login well'
		
		# set current work path
		try:
			cloneFTP.cwd(remotedir)
		except ftplib.error_perm:
			#print '[DEBUG] remote dir is not exist, error'
			return -1
		
		print '[DEBUG]CWD %s well' % remotedir

		# read work path file and download them
		if file == '':
			print cloneFTP.pwd()
			cloneFTP.dir(self.__structure_analysis)
			### code below is trick ###############
			# how to get the callback function value?
			# now use global variables to trans...
			file_list = list(self.__file_list)
			folder_list = list(self.__folder_list)
			#######################################
			# folder list
			for inode in folder_list:
				#print '[DEBUG] in folder %s' % inode
				ele_path = os.path.join(localdir, inode)
				remote_path = os.path.join(remotedir, inode)
				if self.download_file(ele_path, remote_path) == -1:
					cloneFTP.quit()
					return -1
			for inode in file_list:
				#print '[DEBUG] in file %s' % inode
				ele_path = os.path.join(localdir, inode)
				remote_path = os.path.join(remotedir, inode)
				if self.__download_file(ele_path, remote_path, cloneFTP) == -1:
					cloneFTP.quit()
					return -1
		else:
			remote_file = os.path.join(remotedir, file)
			local_file = os.path.join(localdir, file)
			if self.__download_file(local_file, remote_file, cloneFTP) == -1:
				cloneFTP.quit()
				return -1

		cloneFTP.quit()
		return 0
					

	def __structure_analysis(self, information):
		'''
		@summary:
		'''
		inodes = information.split('\n')
		for inode in inodes:
			inode_infor = inode.split()
			print inode_infor
			if inode_infor[0][0] == 'd':
				self.__folder_list.append(inode_infor[-1])
			else:
				self.__file_list.append(inode_infor[-1])
		return

if __name__ == '__main__':
	simpleFtp = simpleFtp('10.81.15.101', '21', 'work', 'ps-testing!!!')
#	simpleFtp.download_file('/home/work/viewdiff/test_download/','/home/work/viewdiff/test/')
	simpleFtp.upload_dir_new('./','/home/work/','fanqiqi_test')
	

