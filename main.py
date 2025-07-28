import socket

import subprocess
import time
import struct
# Path to the UNIX socket file (must match the server’s path)
SOCKET_PATH = "packet"


def mac_addr(mac_bytes):

    return ':'.join(f'{b:02x}' for b in mac_bytes)


def parse_ethernet_frame(frame):
    if len(frame) < 14:
        raise ValueError("Frame too short")
    dest_mac, src_mac, eth_type = struct.unpack('!6s6sH', frame[:14])
    return {
        'dest_mac': mac_addr(dest_mac),
        'src_mac': mac_addr(src_mac),
        'eth_type': hex(eth_type),
        'data' : frame[14:],
    }




def decode_ip(addr):
    return ".".join(map(str,addr))
def parse_ipv4(data):
    d ={};
    version_ihl,tos = struct.unpack("!BB",data[:2])
    d["version"] = version_ihl >> 4
    d["ihl"] = version_ihl &0xf 
    d["tos"] = tos
    d["length"] =struct.unpack("!h",data[2:4])[0]
    d["ttl"] , d["proto"], d["src"], d["dest"] = struct.unpack("!8x B B 2x 4s 4s",data[:20]) 
    try:
        d["src"] = socket.gethostbyaddr(decode_ip( d["src"] ))[0]
    except:
        d["src"] = decode_ip( d["src"] )

    try:
        d["dest"] = socket.gethostbyaddr(decode_ip( d["dest"] ))[0]
    except:
        d["dest"] = decode_ip( d["dest"] )
    return d


def parse_bpf_packet(packet):
    bh_len = struct.unpack_from('H', packet, offset=16)[0]  # bh_hdrlen is at offset 16
    ethernet_frame = packet[bh_len:]  # skip bpf header
    return parse_ethernet_frame(ethernet_frame)



bfd_device = "/dev/bpf0"
subprocess.Popen([f"./out",bfd_device])
# Create a Unix socket
time.sleep(1.0)
client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
    # Connect to the server

    client.connect(SOCKET_PATH)

    # Send data
    i = 1 
    while(i):
        response = client.recv(4096)
        data = parse_bpf_packet(response)
        if  data["eth_type"] != "0x800":
            continue
        ipv4 = parse_ipv4(data["data"])
#        print(data["dest_mac"]+" : "+data["src_mac"]+ " : "+ str(data["eth_type"]))
        print(ipv4) 

finally:
    client.close()
