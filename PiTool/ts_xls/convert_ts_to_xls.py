from xml.dom.minidom import parse
import xml.dom.minidom
import xlrd
import xlwt
import sys
import collections
import glob
import os.path 


def convert_ts_to_xls(infile,inPath,outPath):
	classname = infile.replace(".ts",".xls")
	print("generating excel "+classname+"...")
	#write data to excel
	f = xlwt.Workbook();
	sheet1 = f.add_sheet(u'sheet1',cell_overwrite_ok=True)
	header = [u'name',u'location.filename',u'location.line',u'source',u'translation'];
	col_name = 0;
	col_location_filename = 1;
	col_location_line = 2;
	col_source = 3;
	col_translation = 4;
	for i in range(0,len(header)):
		sheet1.write(0,i,header[i]);
	
	DOMTree = xml.dom.minidom.parse(inPath+"\\"+infile)
	collection = DOMTree.documentElement
	if collection.hasAttribute("language"):
		print("language : %s" % collection.getAttribute("language"))

	# get all context in collection
	contexts = collection.getElementsByTagName("context")

	# print the detail of every context
	row = 1;
	for context in contexts:
		#print "**********context**********"
		name = context.getElementsByTagName('name')[0]
		#print "name: %s" % name.childNodes[0].data
		sheet1.write(row,col_name,name.childNodes[0].data);		
		messages = context.getElementsByTagName('message');
		#print "    ******message******    "
		for message in messages:
			locations = message.getElementsByTagName('location');
			if len(locations)==0:
				continue;
			row = row-1;
			for location in locations:
				row = row+1;
				filename = "";
				line = "";
				if location.hasAttribute("filename"):
					filename = location.getAttribute("filename");
				if location.hasAttribute("line"):
					line = location.getAttribute("line");
				print("    location,filename: %s, line: %s" % (filename,line));
				sheet1.write(row,col_location_filename,filename);
				sheet1.write(row,col_location_line,line);
			source = message.getElementsByTagName('source')[0]
			print("    source: %s" % source.childNodes[0].data)
			sheet1.write(row,col_source,source.childNodes[0].data);		
			translation = message.getElementsByTagName('translation')[0]
			if len(translation.childNodes)>0:
				#print "    translation: %s" % translation.childNodes[0].data
				sheet1.write(row,col_translation,translation.childNodes[0].data);		
			row = row+1;

	f.save(classname)	

inPath = ".\\"
outPath = ".\\model"
if __name__=="__main__":  
    if len(sys.argv)>=2:
    	inPath = sys.argv[1];
    	print("inPath="+inPath)

    if len(sys.argv)>=3:
    	outPath = sys.argv[2];
    	print("outPath="+outPath)

i=0;
for filename in glob.glob(inPath+"\\*.ts"):
	filename=os.path.basename(filename)
	if filename.find("pitool")>=0:
		#print filename
		convert_ts_to_xls(filename,inPath,outPath)
		i=i+1

print("generate "+str(i)+ " excel files.")

