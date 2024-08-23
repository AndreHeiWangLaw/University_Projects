# Name: Andre Hei Wang Law
# ID: 40175600
# Purpose: Server Program
# Author: I am the sole author of this file

import os
import socket
import struct
import sys


# ====================================== Request Function ====================================== #
def handle_client_request(socket, addr, data):
    if len(data) == 0:  # no data, no request, do nothing
        return
    data_byte1 = data[0]  # get 1st byte
    opcode = format(data_byte1 >> 5, '03b')  # get 3bit request opcode, 03b
    # print(opcode)     # used for testing

    # process requests based on opcode number
    if opcode == '000':  # put request
        fl = data[0] & 0b00011111  # file length, b4b3b2b1b0, 5bits
        filename = data[1:fl + 1].decode()  # file name, byte2 to byte fl+1
        fs_temp = data[fl + 1:fl + 5]  # file size, fl+2 to fl+5
        f_size = int.from_bytes(fs_temp, byteorder='big')  # convert to int

        if debugMode:
            print(f'{addr} request put {filename} ({f_size} bytes)')
        f_data = data[fl + 5:]  # rest of data

        count = range(f_size // BUFFER_SIZE_BYTES - 1)  # chunk by buffer_size
        with open(filename, 'wb') as f:
            for i in count:  # write data in chunks based on count range
                f.write(f_data)
                try:  # check connection during data receiving process
                    data = socket.recv(BUFFER_SIZE_BYTES)
                except ConnectionResetError:
                    socket.close()
                    break
            f.write(f_data)
            remainder = (f_size + fl + 5) % BUFFER_SIZE_BYTES  # write remainder bytes
            if remainder > 0:
                f_data = socket.recv(remainder)
                f.write(f_data)

        response = bytearray(1)  # construct response byte array
        response[0] = 0b00000000  # correct put/change req, b7b6b5 res-code, b4b3b2b1b0 unused

    elif opcode == '001':  # get requests
        fl = data[0] & 0b00011111
        filename = data[1:fl + 1].decode()

        if os.path.exists(filename):  # check if exists
            with open(filename, 'rb') as f:  # open, rd: read binary mode
                f_data = f.read()  # read the while file
                f_size_temp = os.path.getsize(filename)  # get file size
                f_size = f_size_temp.to_bytes(4, byteorder="big")  # convert int to 4 bytes

                if debugMode:
                    print(f'{addr} request get {filename} ({f_size} bytes)')

                response = bytearray()  # construct response byte array
                response.extend(data)
                response.extend(f_size)
                response.extend(f_data)

                # send (code similar to client;s send function)
                counter = 0;  # counter for bytes sent
                data_len = len(response)  # total length of data to be sent

                while counter <= data_len:  # loop as long as there is data
                    chunk = response[counter:counter + BUFFER_SIZE_BYTES]  # get chunk of data
                    counter += BUFFER_SIZE_BYTES  # increase counter
                    socket.sendto(chunk, addr)

                remainder = len(response) % BUFFER_SIZE_BYTES  # write remainder bytes
                if remainder > 0:
                    chunk = response[counter:counter + remainder]
                    socket.sendto(chunk, addr)
                return
        else:
            response = bytearray(1)  # construct response byte array
            response[0] = 0b01100000  # file not found, b7b6b5 res-code, b4b3b2b1b0 unused

    elif opcode == '010':  # change request
        current_fl = data[0] & 0b00011111
        current_filename = data[1:current_fl + 1].decode()
        new_fl = (data[current_fl + 1] & 0b00011111)
        new_filename = data[current_fl + 2:current_fl + new_fl + 2].decode()
        response = bytearray(1)  # construct response byte array

        if os.path.isfile(current_filename):
            os.rename(current_filename, new_filename)
            response[0] = 0b00000000  # correct put/change req, b7b6b5 res-code, b4b3b2b1b0 unused
            if debugMode:
                print(f'{addr} request change {current_filename} {new_filename}({current_filename} bytes)')
        else:
            if debugMode:
                print(f'{current_filename} file not found')
            response[0] = 0b01100000  # file not found, b7b6b5 res-code, b4b3b2b1b0 unused
    elif opcode == '011':  # summary request
        fl = data[0] & 0b00011111
        filename = data[1:fl + 1].decode()

        if os.path.exists(filename):
            with open(filename, 'r') as file:
                num = file.readlines()
                num = [float(num.strip()) for num in num]  # txt file w/ new num on new line

                if num:
                    max_num = max(num)
                    min_num = min(num)
                    avg_num = sum(num) / len(num)

                    # Prepare the response with summary information
                    response = bytearray()  # construct response byte array
                    response.append(0b010 << 5 | fl)  # Use the same opcode as the request for response
                    response.extend(filename.encode())
                    response.extend(struct.pack('fff', max_num, min_num, avg_num))

                    counter = 0  # counter for bytes sent
                    data_len = len(response)  # total length of data to be sent

                    while counter <= data_len:  # loop as long as there is data
                        payload = response[counter:counter + BUFFER_SIZE_BYTES]  # get chunk of data
                        counter += BUFFER_SIZE_BYTES  # increase counter
                        socket.sendto(payload, addr)  # send to client

                    remaining_bytes = len(response) % BUFFER_SIZE_BYTES  # write remainder bytes
                    if remaining_bytes > 0:
                        payload = response[counter:counter + remaining_bytes]
                        socket.sendto(payload, addr)
                    return
                else:
                    response = bytearray(1)  # construct response byte array
                    response[0] = 0b01100000  # file not found, b7b6b5 res-code, b4b3b2b1b0 unused
        else:
            response = bytearray(1)  # construct response byte array
            response[0] = 0b01100000  # file not found, b7b6b5 res-code, b4b3b2b1b0 unused
    elif opcode == '100':  # help request
        msg = "bye change get help put summary"
        msg_len = len(msg)  # get filesize using 5 bits

        response = bytearray()  # construct response byte array
        response.append(0b11000000 | msg_len)  # help response
        response.extend(msg.encode())
    else:  # unknown request
        response = bytearray(1)  # construct response byte array
        response[0] = 0b10000000  # unknown request, b7b6b5 res-code, b4b3b2b1b0 unused
    socket.sendto(response, addr)  # send response
    return


# ====================================== User Prompt ====================================== #
try:
    if len(sys.argv) != 4:  # expect 3 arguments
        raise ValueError(f"Expected 3 Arguments. Currently have {len(sys.argv) - 1} arguments")

    protocol = sys.argv[1]  # user input protocol (tcp or udp)
    port = int(sys.argv[2])  # user input server port
    debug = int(sys.argv[3])  # user input debug toggle (0: OFF, 1: ON)

    if protocol not in ['tcp', 'udp']:
        raise ValueError("Expected protocol to be either 'tcp' or 'udp'")
    if not (1 <= port <= 9999):
        raise ValueError("Expected port number between 1 to 9999")
    if debug not in [0, 1]:
        raise ValueError("Expected debug option between 0(OFF) or 1(ON)")

    debugMode = bool(debug)  # convert debug to boolean, 0(OFF-False) or 1(ON-True)

except ValueError as e:
    print(f"\nError: {e}")
    raise SystemExit(f"Command Format: python {sys.argv[0]} <protocol> <port> <debug>\n")

# server IP
serverIP = socket.gethostbyname(socket.gethostname())  # in my case it is 192.168.56.1
#serverIP = "127.0.0.1"  # used to test wireshark
BUFFER_SIZE_BYTES = 2048  # allowed send/receive bytes amount
# current connection information
print(f"\nProtocol: {protocol}, Port: {port}, Debug Mode: {'ON' if debugMode else 'OFF'}, ServerIP: {serverIP}\n")

# ====================================== TCP Socket ====================================== #
if protocol == "tcp":
    tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # tcp: SOCK_STREAM
    tcp_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    tcp_server_socket.bind((serverIP, port))
    tcp_server_socket.listen(5)  # max amount of queued connections

    if debugMode:
        print("Waiting for connections...")  # waiting msg

    while True:
        client_conn, client_addr = tcp_server_socket.accept()  # client connected info

        try:
            if debugMode:
                print(f"TCP Information {client_addr}")  # connected msg
            while True:
                try:
                    data = client_conn.recv(BUFFER_SIZE_BYTES)  # receive data from client
                    if not data:
                        break
                    handle_client_request(client_conn, client_addr, data)
                except ConnectionResetError:
                    print("Closing Connection...")
                    client_conn.close()
                    sys.exit()
        except ConnectionResetError:
            print("Closing Connection...")
            client_conn.close()
            sys.exit()

# ====================================== UDP Socket ====================================== #
if protocol == "udp":
    udp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP: SOCK_DGRAM
    udp_server_socket.bind((serverIP, port))

    if debugMode:
        print("Waiting for connections...")  # waiting msg

    while True:
        data, client_addr = udp_server_socket.recvfrom(BUFFER_SIZE_BYTES)  # client connected info

        handle_client_request(udp_server_socket, client_addr, data)