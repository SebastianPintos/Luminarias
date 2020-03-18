import socket
#import redis

UDP_IP = "0.0.0.0"
UDP_PORT = 8888
cont = 0;
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
#r = redis.Redis(host='localhost',port=6379,db=0)

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print(data)
    #r.set("packet "+ str(cont),data) 
    cont+=1

