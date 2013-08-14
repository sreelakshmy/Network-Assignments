from pyftpdlib.authorizers import DummyAuthorizer			#Python FTP server library
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer						

authorizer = DummyAuthorizer()						#dummy authorizer

authorizer.add_anonymous("/home/sree/Desktop/",perm="elradfmw")		#adding a user(userid,password) here as anonymous to the server with 										read , write etc permission

handler = FTPHandler							
handler.authorizer = authorizer

connection= ('192.168.1.5' , 21)					#host address and port number

server = FTPServer(connection, handler)					

server.serve_forever()
