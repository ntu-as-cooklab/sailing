import socket
import struct

## TCP sender class
class TCPsender:
    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(('localhost', port))

    def __del__(self):
        self.sock.close()

# Main execution

tcpSender = TCPsender(1234) # Port 1234

while 1:
    pkg = ('a', 'b', 'c', 'd', 'e')
    packer = struct.Struct('c c c c c')
    packed_data = packer.pack(*pkg)
    tcpSender.sock.sendall(packed_data)
