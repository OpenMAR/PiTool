#! python3
import os
import sys
import shutil
import datetime
from ftplib import FTP

def set_title(title):
	os.system('title '+title)

class Xfer(object):
	'''''
	@note: upload local file or dirs recursively to ftp server
	'''
	def __init__(self):
		self.ftp = None
		
	def __del__(self):
		pass
		
	def setFtpParams(self, ip, uname, pwd, port = 21, timeout = 60):
		self.ip = ip
		self.uname = uname
		self.pwd = pwd
		self.port = port
		self.timeout = timeout
		
	def initEnv(self):
		if self.ftp is None:
			self.ftp = FTP()
			print('### connect ftp server: %s ...'%self.ip)
			self.ftp.connect(self.ip, self.port, self.timeout)
			self.ftp.login(self.uname, self.pwd)
			print(self.ftp.getwelcome())
		
	def clearEnv(self):
		if self.ftp:
			self.ftp.close()
			print('### disconnect ftp server: %s!'%self.ip)
			self.ftp = None
	
	def uploadDir(self, localdir='./', remotedir='./'):
		if not os.path.isdir(localdir):
			return
		self.ftp.cwd(remotedir)
		for file in os.listdir(localdir):
			src = os.path.join(localdir, file)
			if os.path.isfile(src):
				self.uploadFile(src, file)
			elif os.path.isdir(src):
				try:
					self.ftp.mkd(file)
				except:
					sys.stderr.write('the dir is exists %s'%file)
				self.uploadDir(src, file)
		self.ftp.cwd('..')
		
	def makeDir(self, dir_name, remotedir='./'):
		self.ftp.cwd(remotedir)
		try:
			self.ftp.mkd(dir_name)
		except:
			sys.stderr.write('the dir is exists %s'%dir_name)
		
	def uploadFile(self, localpath, remotepath='./'):
		if not os.path.isfile(localpath):
			return
		print('+++ upload %s to %s:%s'%(localpath, self.ip, remotepath))
		self.ftp.storbinary('STOR ' + remotepath, open(localpath, 'rb'))
		
	def downloadFile(self,remotepath, localpath):
		print('+++ download %s to %s:%s'%(remotepath, self.ip, localpath))
		fp = open(localpath, 'wb')
		self.ftp.retrbinary('RETR %s' % remotepath, fp.write)
		
		
	def get_lastest_path(self,poll_path):
		xfer = Xfer()
		xfer.setFtpParams('10.10.1.250', 'suki.su', '123456')
		xfer.initEnv()
		dirs_info = []
		xfer.ftp.dir(poll_path, dirs_info.append)
		dirs = [f.split()[-1] for f in dirs_info if f.startswith('d')]
		lastest_dir = ''
		for dir in dirs:
			if dir.startswith('2') and dir > lastest_dir:
				lastest_dir = dir
		xfer.clearEnv()
		return poll_path + lastest_dir
	def __filetype(self, src):
		if os.path.isfile(src):
			index = src.rfind('\\')
			if index == -1:
				index = src.rfind('/')
			return _XFER_FILE, src[index+1:]
		elif os.path.isdir(src):
			return _XFER_DIR, ''
	
	def upload(self, src):
		filetype, filename = self.__filetype(src)
		
		self.initEnv()
		if filetype == _XFER_DIR:
			self.srcDir = src
			self.uploadDir(self.srcDir)
		elif filetype == _XFER_FILE:
			self.uploadFile(src, filename)
		self.clearEnv()
	
def copy_tool_files():
	print('copy tool files')
	if os.path.exists(out_tool_dir):
		shutil.rmtree(out_tool_dir)
	os.makedirs(out_tool_dir)
	os.makedirs(out_tool_dir+'\\DLL')
	os.makedirs(out_tool_dir+'\\language')
	shutil.copy(release_64_path+'\\PiTool.exe',out_tool_dir)
	shutil.copy(piSvc_dll_path+'\\PiSvc.dll',out_tool_dir+'\\DLL\\')
	shutil.copy('PiTool'+'\\pitool_cn.qm',out_tool_dir+'\\language\\')
	shutil.copy('PiTool'+'\\pitool_en.qm',out_tool_dir+'\\language\\')
	shutil.copytree('PiTool_setup\\PiService',out_tool_dir+'\\PiService')
	shutil.copytree('PiTool_setup\\PvrRuntime',out_tool_dir+'\\PvrRuntime')
	shutil.copytree('PiTool_setup\\qt',out_tool_dir+'\\qt')
	shutil.copytree('PiTool_setup\\Tools',out_tool_dir+'\\Tools')
	shutil.copytree('PiTool_setup\\Font',out_tool_dir+'\\Font')
	shutil.copytree('PiTool_setup\\dfudriver',out_tool_dir+'\\dfudriver')
	
def copy_tool_tob_files():
	if os.path.exists(out_tool_dir):
		shutil.rmtree(out_tool_dir)
	os.makedirs(out_tool_dir)

def pack_tool_files(version):
	print('packing tool files')
	inno_script_compile('PiTool',version)
	#ret = os.system(zip_cmd+' a -sfx7z.sfx '+setup_now_dir+'\\debug.exe'+' '+out_server_dir+'\\PiService.pdb')
	#if ret != 0:
	#	print('!!!ERROR:zip debug files failed!')
	#	quit()	

def pack_tool_tob_files(compile_name = 'PiTool'):
	print('packing tool files')

def inno_script_compile(file,version):
	print('compiling:',file)
	os.system(signtool + ' ' + 'PiTool_setup\\PiService\\*')
	os.system(signtool + ' ' + 'PiTool_setup\\PvrRuntime\\*')
	os.system(signtool + ' ' + 'PiTool_setup\\Tools\\*')
	
	ret = os.system(inno_compiler+' /o'+compile_path+'\\'+setup_now_dir+' /F'+file+'Setup_'+ version +' /dMyInputPath='+compile_path + ' /dMyAppVersion='+version +' PiTool_setup\\inno-script\\'+file+'.iss')
	if ret != 0:
		print('inno compiling failed')
		quit()
	os.system(signtool + ' ' + setup_now_dir+'\\*')
	
def	upload_tool_tob_files(filename='PiTool'):
	print('uploading tool files')

set_title('packing files')

print('making dirs')
if not os.path.exists('out'):
	os.makedirs('out')
#	shutil.rmtree('out')
#if os.path.exists('out'):
#	print('!!!ERROR:clean out dir failed!')
#	exit(1)
#os.makedirs('out')
out_tool_dir = 'out\\tool'
out_tool_tob_dir = 'out\\tool_tob'

now = datetime.datetime.now()
str_now = now.strftime('%Y-%m-%d-%H-%M-%S')

if not os.path.exists('setup'):
	os.makedirs('setup')
setup_now_dir = 'setup\\'+str_now
os.makedirs(setup_now_dir)
	
compile_path = os.getcwd()
release_64_path = 'PiTool\\release'
piSvc_dll_path = 'PiTool\\piSvc'
inno_compiler = 'PiTool_setup\\Inno-Setup-5\\ISCC.exe'
signtool = 'PiTool_setup\\SignTool\\pitool.bat'
zip_cmd = 'PiTool_setup\\7zip\\7z.exe'
if len(sys.argv) >= 3 and sys.argv[1] == 'toC':
	copy_tool_files()
	pack_tool_files(sys.argv[2])
else:
	quit()