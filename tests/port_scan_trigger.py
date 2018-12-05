import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 5000
MESSAGE = "abc123"

#This just sends UDP data to the port that is being listened on.
#In the case of the checker it is UDP port 5000

while True:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))