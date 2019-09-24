#! python3
import os
import sys

	
def buildPiService():
	path = os.path.abspath('./PiService/')
	ret = os.system(path + '/build_pack_server_to_tool.cmd')
	if ret != 0:
		print('ERROR: service compile failed!')
		exit(1)
	
print('\n\nbuild complated!')

buildPiService()