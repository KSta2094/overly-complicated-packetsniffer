import socket
from socket import htons
import struct
# Path to the UNIX socket file (must match the server’s path)
SOCKET_PATH = "packet"

# Create a Unix socket
client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
    # Connect to the server

    client.connect(SOCKET_PATH)

    # Send data
    response = client.recv(4096)
    for i in range(7,14):
        print(hex(htons(response[i])))

finally:
    client.close()
