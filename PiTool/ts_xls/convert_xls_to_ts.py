from xml.dom.minidom import parse
import xml.dom.minidom
import xlrd
import xlwt
import sys
import collections
import glob
import os.path 
import os

def mkdir(path):
    path=path.strip()
    path=path.rstrip("\\")
    isExists=os.path.exists(path)
    if not isExists:
        os.makedirs(path) 
        print(path+' mkdir success')
        return True
    else:
        print(path+' dir is exists')
        return False

def convert_xls_to_ts(infile,inPath,outPath):
	classname = infile.replace(".xls",".ts")
	print("generating ts "+classname+"...")
	language="zh_CN";
	if infile.find("en")>=0:
		language = "en_US";
	elif infile.find("jp")>=0:
		language = "ja_JP";
		
	output = '<?xml version="1.0" encoding="utf-8"?>\n';
	output = output+'<!DOCTYPE TS>\n';
	output = output+'<TS version="2.1" language="%s" sourcelanguage="zh_CN">\n' % language;
	hOutfile = open(outPath+"\\"+classname,"w");
	hOutfile.write(output);

	#read data to excel
	workbook = xlrd.open_workbook(inPath+"\\"+infile);
	sheet1 = workbook.sheet_by_index(0);
	col_name =-1;
	col_location_filename=-1;
	col_location_line = -1;
	col_source = -1;
	col_translation = -1;


	for col in range(0,sheet1.ncols):
		header = sheet1.cell(0,col).value;
		if header=="name":
			col_name = col;
		elif header=="location.filename":
			col_location_filename = col;
		elif header=="location.line":
			col_location_line = col;
		elif header=="source":
			col_source = col;
		elif header=="translation":
			col_translation = col;
	print(col_name,col_location_filename,col_location_line,col_source,col_translation)
	if col_name==-1 or col_location_filename==-1 or col_location_line==-1 or col_source==-1 or col_translation==-1:
		print(infile,"xls is not ts format");
		return;
	hasContext = 0;
	hasMessage = 0;
	for row in range(1,sheet1.nrows):
		name = sheet1.cell(row,col_name).value;
		filename = sheet1.cell(row,col_location_filename).value;
		line = sheet1.cell(row,col_location_line).value;
		source = sheet1.cell(row,col_source).value.encode('utf-8').replace(b"&",b"&amp;");
		source = source.replace(b"<",b"&lt;");
		source = source.replace(b">",b"&gt;");
		translation = sheet1.cell(row,col_translation).value.encode('utf-8').replace(b"&",b"&amp;");
		translation = translation.replace(b"<",b"&lt;");
		translation = translation.replace(b">",b"&gt;");
		print(name,filename,line,source,translation);
		if name!="":
			if hasContext==1:
				hOutfile.write("</context>\n");
				hasContext = 0;
			hOutfile.write("<context>\n\t<name>%s</name>\n" % name);
			hasContext = 1;
		if hasMessage==0:
			hOutfile.write("\t<message>\n");
			hasMessage = 1;
		string = '\t\t<location filename="%s" line="%s"/>\n' % (filename,line);
		hOutfile.write(string);
		if source!="":
			string = '\t\t<source>%s</source>\n' % source;
			
			hOutfile.write(string);
			hOutfile.write("\t\t<translation>%s</translation>\n" % translation);
			hOutfile.write("\t</message>\n");
			hasMessage = 0;
	if hasContext==1:
		hOutfile.write("</context>\n");
	hOutfile.write("</TS>\n");
	hOutfile.close();
	
inPath = ".\\"
outPath = ".\\ts_backup"
mkdir(outPath);
if __name__=="__main__":  
    if len(sys.argv)>=2:
    	inPath = sys.argv[1];
    	print("inPath="+inPath)

    if len(sys.argv)>=3:
    	outPath = sys.argv[2];
    	print("outPath="+outPath)

i=0;
for filename in glob.glob(inPath+"\\*.xls"):
	filename=os.path.basename(filename)
	if filename.find("pi")>=0:
		#print filename
		convert_xls_to_ts(filename,inPath,outPath)
		i=i+1

print("generate "+str(i)+ " ts files.")

