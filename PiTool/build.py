import os
import sys

def buildAllPlatformProject(project, platforms, configurations):
	for platform in platforms:
		for configuration in configurations:
			buildProject(project, platform, configuration)
			
	print(project + ' build successful!')

def buildProject(project, platform, configuration):
	os.system('title ' + project + ' ' + platform + ' ' + configuration)
	ret = os.system('build.bat ' + project + ' ' + platform + ' ' + configuration)
	if ret != 0:
		print('>>>>>>>>>>>>>>>>>>> ERROR!!! >>>>>>>>>>>>>>>> ' + project + ' ' + platform + ' ' + configuration)
		exit(1)

def generateLanguageResrouce(language_files):
	for language in language_files:
		ret = os.system('ts_to_pm.bat ' + language)
		if ret != 0:
			print('Error : generate ' + language + " language failed!!!")
			exit(1)
	
	print('language generation successful!')
	
language_files = {'pitool_cn','pitool_en','pitool_jp','pitool_ge'}		
build_project = 'PiTool.vcxproj'		
all_platforms = {'x64'}		
all_configurations = {'Release', 'Debug'}	
if len(sys.argv) == 2 and sys.argv[1] == 'all':
	generateLanguageResrouce(language_files)
	buildAllPlatformProject(build_project,all_platforms,all_configurations)
elif len(sys.argv) == 2 and sys.argv[1] == 'release':
	generateLanguageResrouce(language_files)
	buildAllPlatformProject(build_project,all_platforms,{'Release'})
elif len(sys.argv) == 2 and sys.argv[1] == 'debug':
	generateLanguageResrouce(language_files)
	buildAllPlatformProject(build_project,all_platforms,{'Debug'})
else: 
	quit()
	
print('\n\nbuild completed!')
