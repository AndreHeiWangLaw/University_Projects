# Nathan McDonald-Fortier ID: 40134141
# Tim Freiman - 40091639

# This is the server side of the project

# All code is original and written by us

import sys
import socket
import os
import time


BUFFER_SIZE = 4096

def main(args):
    (conn,ip,port,debug) = parse_cli(args)
# Start server using TCP/UDP, IP and port with debug flag

################################# TCP MODE ##################################################################
    # Start server using TCP/UDP, IP and port with debug flag.
    if conn == socket.SOCK_STREAM:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)

        server_socket.bind((ip, port))
        server_socket.listen(1)  # Listen for 1 connection.
        if debug:
            print(f'TCP server listening on {ip}:{port}')
        while True:
            client_socket, client_address = server_socket.accept()
            if debug:
                print(f"TCP connection established with {client_address}")
            while True:
                try:
                    data = client_socket.recv(BUFFER_SIZE)
                except ConnectionResetError:
                    client_socket.close()
                    break
                if not data:
                    if debug:
                        print(f"No data received from {client_address} closing connection")
                    client_socket.close()
                    break
                if debug:
                    print(f"TCP connection established with {client_address}")
                handle_request(client_socket, client_address, data, debug)
            time.sleep(2)
            client_socket.close()


    else:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_socket.bind((ip, port))
        if debug:
            print(f'UDP server listening on {ip}:{port}')

        while True:
            data, client_address = server_socket.recvfrom(BUFFER_SIZE)  
            if debug:
                print(f"UDP packet received from {client_address}")
            handle_request(server_socket, client_address, data, debug)

def handle_request(client_socket, client_address, data, debug):
    if len(data) == 0:
        return
    opcode = (data[0] & 0b11100000)>>5
    # PUT request.
    if opcode == 0:
        filename_length = data[0] & 0b00011111
        filename = data[1:filename_length+1].decode()
        file_size = int.from_bytes(data[filename_length+1:filename_length+5], byteorder='big')
        if debug:
            print(f'Received PUT request from {client_address}: {filename} ({file_size} bytes)')
        file_data = data[filename_length+5:]
        count = range((file_size) //BUFFER_SIZE-1)
        with open(filename, 'wb') as f:
            for i in count:
                f.write(file_data)
                
                try:
                    data = client_socket.recv(BUFFER_SIZE)
                except ConnectionResetError:
                    client_socket.close()
                    break
            f.write(file_data)    
            remaining_bytes = (file_size+filename_length+5) % BUFFER_SIZE
            if remaining_bytes > 0:
                file_data = client_socket.recv(remaining_bytes)
                f.write(file_data)
        response = bytearray(1)
        response[0] = 0b00000000  # Set response code to 000.
        data=0

    # GET request.
    elif opcode == 1: 
        filename_length = data[0] & 0b00011111
        filename = data[1:filename_length+1].decode()

        if os.path.exists(filename):
            with open(filename, 'rb') as f:
                file_data = f.read()
                file_size = len(file_data).to_bytes(4, byteorder='big')
                if debug:
                    print(f'Sending {filename} ({len(file_data)} bytes) to {client_address}')
                response = bytearray()
                response.extend(data)  # 001 response is same as request opcode for correct GET request.
                response.extend(file_size)
                response.extend(file_data)

                bytes_sent = 0
                bytes_to_send = len(response)
                while bytes_sent <= bytes_to_send:
                    payload = response[bytes_sent:bytes_sent+BUFFER_SIZE]
                    bytes_sent += BUFFER_SIZE
                    client_socket.sendto(payload, client_address)
                remaining_bytes = len(response) % BUFFER_SIZE
                if remaining_bytes > 0:
                    payload = response[bytes_sent:bytes_sent+remaining_bytes]
                    client_socket.sendto(payload, client_address)
                return
        else:
            response = bytearray(1)
            response[0] = 0b01000000  # Set response code to 010 Error-File Not Found.



    # CHANGE request.
    ################################ CHANGE REQUEST #################################################################
    elif opcode == 2:
        #change operation
        old_filename_length = data[0] & 0b00011111
        old_filename = data[1:old_filename_length+1].decode()
        new_filename_length = (data[old_filename_length+1] & 0b00011111)
        new_filename = data[old_filename_length+2:old_filename_length+new_filename_length+2].decode()
        
        response = bytearray(1)
        if os.path.isfile(old_filename):
            os.rename(old_filename, new_filename)
            response[0]  = 0b00000000
            if debug:
                print(f'Received CHANGE request from {client_address}: {old_filename} -> {new_filename}')
        else:
            if debug:
                print(f'{old_filename} file not found')
            response[0] = 0b10100000

    ################################ HELP REQUEST #################################################################
    elif opcode == 3:
        #help
        help_string = "Commands: get, put, change"
        help_len = len(help_string) #get filesize using 5 bits
        response = bytearray()
        response.append(0b11000000 | help_len) #110 HELP response
        response.extend(help_string.encode())
    ################################ UNKNOWN REQUEST #################################################################
    else:
        #unkn
        response = bytearray(1)
        response[0] = 0b11000000

    ################################ SEND REQUEST #################################################################
    #send response

    client_socket.sendto(response,client_address)
    return


def parse_cli(args):
    # parse connection type
    conn = args[1]
    if conn.lower() == "udp": conn = socket.SOCK_DGRAM
    elif conn.lower() == "tcp": conn = socket.SOCK_STREAM
    else: sys.exit("Error: Invalid connection type")

    # parse ip
    ip = args[2]
    temp = ip.split('.')
    if len(temp) < 4: sys.exit("Error: Invalid IP address")
    for num in temp:
        if int(num) < 0 or int(num) > 255:
            sys.exit("Error: Invalid IP address")

    # parse port
    port = int(args[3])
    if port < 1 or port > 65535:
        sys.exit("Error: Invalid port number")

    # parse debug
    debug = args[4]
    if debug == 1: debug = True
    else: debug = False

    return (conn, ip, port, debug)


if __name__ == "__main__":
    if len(sys.argv) < 5: 
        sys.exit("Missing arguments. Usage: 'client.py [tcp/udp] [ip] [port] [0/1](debug)'")
    main(sys.argv)
