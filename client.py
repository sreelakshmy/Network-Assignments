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

