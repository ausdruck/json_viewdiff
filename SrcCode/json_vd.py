import os,sys,time,commands
from ConfigParser import *
sys.path.append("commonLib")
from simpleFtp import *

class Json_view:
	'''
	@summary:jsonview
	'''
	def __init__(self,conf_file="auto_jsonvd.conf"):
		self._config = ConfigParser()
		self._config.read(conf_file)
		self.host = self._config.get("server_info","host")
		self.port = self._config.get("server_info","port")	
		self.username = self._config.get("server_info","username")
		self.passwd = self._config.get("server_info","passwd")
		self.remote_dir = self._config.get("server_info","remote_dir")
		self.key = self._config.get("server_info","key")
		pass
	def sendtoserver(self,new_input='',old_input='',job_name=''):
		if not os.path.isfile(new_input):
			print '[FATAL]%s is not a file' % new_input
			return -1  
		if not os.path.isfile(old_input):
			print '[FATAL]%s is not a file' % old_input
			return -1 
		if job_name=='':
			job_name=time.strftime('%Y%m%d%H%M%S')
					
		status ,ret = commands.getstatusoutput('cp %s tools/new_input.org;cp %s tools/old_input.org;mkdir -p tools/%s' % (new_input,old_input,job_name))
		if status != 0:
			print '[FATAL]cp input files fail!'
			return -1
		status ,ret = commands.getstatusoutput('cd tools; ./main -s new_input.org old_input.org -k %s;mv *.log %s;mv *.org %s' % (self.key,job_name,job_name) )
		if status != 0:
			print '[FATAL]tools ./main maybe core !input illegal!'
			return -1
		#print job_name
		simple_ftp = simpleFtp(self.host,self.port,self.username,self.passwd)
		path = os.path.abspath("./")
		#print '%s/tools/%s' % (path,job_name)
		ret=simple_ftp.upload_dir_new('%s/tools/%s' % (path,job_name),self.remote_dir,job_name)
		commands.getstatusoutput('rm -rf tools/%s' % job_name ) 
		if(self.remote_dir.split('/')[-1]==""):
			user_name=self.remote_dir.split('/')[-2];
		else:
			user_name=self.remote_dir.split('/')[-1];
		if(ret!=-1):
			print "{\"status\":true,\"info\":\"http://%s:8000/json_vd.new/pi_get.php?username=%s&job_flag=%s\"}" %(self.host,user_name,job_name)
if __name__ == '__main__':
	if (len(sys.argv)!=3 and len(sys.argv)!=4):
		print '**********************help**************\
			\n******input like: python json_vd.py oldinputfile newinputfile (jobname)*****'
		exit(0)
	_new_input = sys.argv[2]
	_old_input = sys.argv[1]
	if(len(sys.argv)==4):
		job= sys.argv[3]
	else :
		job=""
	test = Json_view()
	test.sendtoserver(new_input=_new_input,old_input=_old_input,job_name=job)
