from ftplib import FTP								
import sys
import shutil

host= '192.168.1.5'						# address of the host
port= 21							# port for FTP system in TCP Socket
ftp = FTP()
ftp.connect(host, port)						# opening FTP connection	
print ftp.getwelcome()						# welcome message sent by the server in reply to the initial connection.
ftp.login()							#Log in as the given user (username,password) , here loging in as anonymous

print ("The File contents in the current working directory : ")	# listing all file in the current working directory(remote)
ftp.retrlines('LIST')						## list directory contents <retrlines>Retrieve a file or directory listing in 									ASCII transfer mode.


#print ftp.pwd(ftp)
						
sources = "/home/sree/Desktop/"+sys.argv[1];	    # File transfer (downloading) from Server to Client pass file name as command line argument
dscc = "/home/sree/networking/nw_lab/"
def download(ftp, filenames):
    shutil.copy2(filenames,dscc)				#get the file to the file or directory.
print ("Server to Client file transfer completed!");
download(ftp,sources)

sourcec = "/home/sree/networking/nw_lab/"+sys.argv[2];# File transfer (uploading) from Client to Server pass file name as command line argument
dscs = "/home/sree/Desktop/"
def upload(ftp, filenamec):
    shutil.copy2(filenamec,dscs)				#get the file to the file or directory.
print ("Cient to Server file transfer completed!");
upload(ftp,sourcec)


