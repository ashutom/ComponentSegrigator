import sys
import os
import platform


#PathPrefix="C:\\Users\\ashmishr\\OneDrive - Advanced Micro Devices Inc\\Desktop\\bug\\SWDEV-358001\\ProfilerAnalysis\\Writing Code"

CREATING_FILE_NAME=str("Compared_files.csv")

INVALID_INDEX=-1

def CheckFileAndPermissions(filelist):
	
	try:
		if 3> len(filelist):
			raise Exception(" Less number of files sent for evaluation ")
		fp=open(filelist[2],'w')
		fp.close()
		os.remove(filelist[2])
		for counter in 0,1:
			file=filelist[counter]
			if False == os.path.isfile(file):
				raise Exception(f"  Exception :  {file} is not present or does not have permissions for reading / writing ")
			f=open(file)
			f.close()
	except Exception as exp:
		print("File Exception Occurred in  CheckFileAndPermissions ")
		print(exp)
		sys.exit(-1)


def Usage():
	
	arr=list()
	slash=str("/")
	if "Windows" == platform.system():
		slash=str("\\")

	print(f" This script compares the output of component identifier tool by making a new csv file in which the data of second file is patched to first one based on kernels \n\n\n")
	print(f" Two ways to execut this script  : \n\n")
	print(f" 1)  Supply two file names as arguments to this script \n\n")
	print(f" 2)  Execute the script in a directory where you have both files and let the script choose which one to be patched to which \n\n")

	choice = input(" Enter Choice  (1/2) [anything else shall exit] : ").strip()

	if choice == str("2"):
		PathPrefix = input(" Enter path of the directory to find the files ").strip()
		arr = os.listdir(PathPrefix)
		numberoffilesindir=len(arr)
		if 2!=numberoffilesindir:
			print(" There are {numberoffilesindir} which is not equal to 2 ")
			sys.exit(-2)

		arr[0]=PathPrefix.strip()+slash+arr[0]
		arr[1]=PathPrefix.strip()+slash+arr[1]
		third=""
		third=PathPrefix.strip()+slash+CREATING_FILE_NAME.strip()
		arr.append(third)
	elif choice == str("1"):
		filename=input("Enter First file name ").strip()
		arr.append(filename)
		filename=input("Enter Second file name ").strip()
		arr.append(filename)
		filename=input("Enter Name of the file which needs to be generated ").strip()
		arr.append(filename)
	else:
		sys.exit(0)

	CheckFileAndPermissions(arr)
	
	return arr


def getKernelStartEndID(line):
	end=INVALID_INDEX
	start=INVALID_INDEX
	end=len(line)-1
	times=0
	while end>=0:
		if line[end]==',':
			times=times+1
		if times==3:
			end=end-1
			break
		end=end-1
	
	if times<3:
		return start,end
	
	start=0
	while start<end:
		if line[start]==',':
			start=start+1
			break
		start=start+1
	if start>=end or start==0:
		end=start=INVALID_INDEX
	
	return start , end


def getDictonaryByKernel(file):
	mydic=dict()
	with open(file) as f:
		for line in f:
			#print(line)
			s,e=getKernelStartEndID(line)
			if s!=INVALID_INDEX and e!=INVALID_INDEX:
				dickey=line[s:e+1].strip()
				dickval=line[e+2:].strip()
				dickval=dickval.replace(" ","")
				mydic[dickey]=dickval
			
		f.close()

	return mydic


def	mergetwotoone(one,two):
	for k in one:
		if k in two:
			tmpstr=str(one[k]).strip()+str(",")+str(two[k]).strip()+str("\n")
			if k == 'Kernel':
				tmpstr=str(one[k]).strip()+str(",AvgExeTime_2,NoOfTimesAppearedInExe_2,TotalTimeOfExe_2")+str("\n")
			del two[k]
		else:
			tmpstr=str(one[k]).strip()+str(",0,0,0").strip()+str("\n")
		
		one[k]=tmpstr

	for k2 in two:
		one[k2]=str("0,0,0,").strip()+str(two[k2]).strip()+str("\n")

	return one


def writetofile(tofile,fromfile,fiandic):
	openedwritefile=open(tofile,'w')
	lastid=0
	with open(fromfile) as f:
		for line in f:
			#print(line)
			s,e=getKernelStartEndID(line)
			if s!=INVALID_INDEX and e!=INVALID_INDEX:
				dickey=line[s:e+1].strip()
				print(dickey)
				if dickey in fiandic:
					print(fiandic[dickey])
					wstring=line[:s].strip()+dickey.strip()+str(",")+str(fiandic[dickey])
					#wstring=wstring+str("\n")
					openedwritefile.write(wstring)
					print(wstring)
					del fiandic[dickey]
			lastid=lastid+1
	
	lastid=lastid-1
	for key in fiandic:
		lastid=lastid+1
		wstr=str(lastid)+str(",")+str(key)+str(",")+str(fiandic[key])
		openedwritefile.write(wstr)
		print(wstr)
		#del fiandic[key]
	
	openedwritefile.close()


## start main
if __name__=="__main__":

	arr=Usage()
	diclist=[]
	filelist=[]
	for counter in 0,1:

		filelist.append(arr[counter])
		di=getDictonaryByKernel(arr[counter])
		diclist.append(di)

	fdic=mergetwotoone(diclist[0],diclist[1])
	#wfile=str(PathPrefix)+str("\\")+str("Compared_files.csv")
	#wfile=str("Compared_files.csv")
	writetofile(arr[2],filelist[0],fdic)
	



