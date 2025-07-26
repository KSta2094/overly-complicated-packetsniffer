import socket

# Path to the UNIX socket file (must match the server’s path)
SOCKET_PATH = "packet"

# Create a Unix socket
client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
    # Connect to the server
    client.connect(SOCKET_PATH)
    while(1):
        cin = input("> ");
        # Send data
        client.sendall(cin.encode())

        # Receive response
        response = client.recv(4096)
        if(-1!=response.find(b"CLOSING SOCKET")):
            print(response.decode())
            break;
        print("Recived: "+response.decode())

finally:
    client.close()
