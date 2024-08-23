# Name: Andre Hei Wang Law
# ID: 40175600
# Purpose: Client Program
# Author: I am the sole author of this file

import os
import socket
import struct
import sys


# ====================================== Response Function ====================================== #
def handle_server_response():
    data = clientSocket.recv(BUFFER_SIZE_BYTES)  # receive data from client socket
    if len(data) == 0:
        data = clientSocket.recv(BUFFER_SIZE_BYTES)
    data_byte1 = data[0]  # get 1st byte
    res_code = format(data_byte1 >> 5, '03b')  # get 3bit response code, 03b

    if res_code == '000':  # response for correct put/change request
        print("Correct 'put' or 'change' request")
    elif res_code == '001':  # response for correct get request
        # segment the data
        fl = data[0] & 0b11111  # file length, b4b3b2b1b0, 5bits
        filename = data[1:fl + 1]  # file name, byte2 to byte fl+1
        fs_temp = data[fl + 1:fl + 5]  # file size, fl+2 to fl+5
        f_size = int.from_bytes(fs_temp, 'big')  # convert to int

        count = range(f_size // BUFFER_SIZE_BYTES - 1)  # chunk by buffer_size
        with open(filename, 'wb') as f:
            for i in count:  # loop, write file for 'count' range times
                f_data = clientSocket.recv(BUFFER_SIZE_BYTES)
                f.write(f_data)

            remainder = (f_size + fl + 5) % BUFFER_SIZE_BYTES  # write remainder bytes
            if remainder > 0:
                f_data = clientSocket.recv(remainder)
                f.write(f_data)
        print("Correct 'get' request")
    elif res_code == '010':  # response of statistical summary
        fl = data[0] & 0b00011111  # file length, b4b3b2b1b0, 5bits
        summary_data = data[fl + 1:]  # rest of the data

        # unpack three floats ('fff') from the summary_data
        max_num, min_num, avg_num = struct.unpack('fff', summary_data)
        print('Statistical summary')
        print(f'Max: {max_num}, Min: {min_num}, Avg: {avg_num}')
    elif res_code == '011':  # error-file not found
        print("File not found error")
    elif res_code == '100':  # error-unknown request
        print("Unknown request error")
    elif res_code == '101':  # response for unsuccessful change
        print("Change request unsuccessful error")
    elif res_code == '110':  # help-response
        fl = data[0] & 0b11111
        help_msg = data[1:fl + 1].decode()
        print("Commands are: " + help_msg)
    else:  # else
        print("Wrong Opcode")
    return


# ====================================== Send Function ====================================== #
def send(data):
    global serverIP, port
    counter = 0;  # counter for bytes sent
    data_len = len(data)  # total length of data to be sent

    while counter <= data_len:  # loop as long as there is data
        chunk = data[counter:counter + BUFFER_SIZE_BYTES]  # get chunk of data
        counter += BUFFER_SIZE_BYTES  # increase counter
        clientSocket.sendto(chunk, (serverIP, port))  # send to server

    remainder = len(data) % BUFFER_SIZE_BYTES  # write remainder bytes
    if remainder > 0:
        chunk = data[counter:counter + remainder]
        clientSocket.sendto(chunk, (serverIP, port))


# ====================================== Commands Functions ====================================== #
def put(cmd):  # request to put file on server
    filename = cmd[1]  # get filename, user argument index 1
    fl = len(filename)  # get length

    if fl > 31:  # reject if filename exceeds 31 characters
        print("Filename too long (max:31)")
        return

    if os.path.exists(filename):  # check if exists
        with open(filename, 'rb') as f:  # open, rd: read binary mode
            f_data = f.read()  # read the while file
            f_size_temp = os.path.getsize(filename)  # get file size
            f_size = f_size_temp.to_bytes(4, byteorder="big")  # convert int to 4 bytes
            request = bytearray()  # create byte array to construct request msg
            request.append(0b000 << 5 | fl)  # opcode 000, put
            request.extend(filename.encode())
            request.extend(f_size)
            request.extend(f_data)
    else:
        if debug:
            print("File not found error")
        return
    send(request)  # send request to server function
    handle_server_response()  # handle server's response function
    return


def get(cmd):
    filename = cmd[1]  # get filename, user argument index 1
    fl = len(filename)  # get length
    request = bytearray()  # create byte array to construct request msg
    request.append(0b001 << 5 | fl)  # opcode 001, get
    request.extend(filename.encode())  # append
    send(request)  # send request to server function
    handle_server_response()  # handle server's response function
    return


def change(cmd):
    current_filename = cmd[1]  # get current filename, user argument index 1
    current_fl = len(current_filename)  # current filename length
    new_filename = cmd[2]  # get new filename, user argument index 1
    new_fl = len(new_filename)  # new filename length
    request = bytearray()  # create byte array to construct request msg
    request.append(0b010 << 5 | current_fl)  # opcode 010, change
    request.extend(current_filename.encode())  # append current filename
    request.append(0b000 << 5 | new_fl)  # opcode 000, put
    request.extend(new_filename.encode())  # append new filename
    send(request)  # send request to server function
    handle_server_response()  # handle server's response function
    return


def summary(cmd):
    filename = cmd[1]  # get filename, user argument index 1
    fl = len(filename)  # get length
    request = bytearray()  # create byte array to construct request msg
    request.append(0b011 << 5 | fl)  # opcode 011, summary
    request.extend(filename.encode())  # append
    send(request)  # send request to server function
    handle_server_response()  # handle server's response function
    return


def help():
    request = bytearray()  # create byte array to construct request msg
    request.append(0b10000000)  # append
    send(request)  # send request to server function
    handle_server_response()  # handle server's response function
    return


def bye():
    if protocol == "tcp":
        clientSocket.shutdown(socket.SHUT_RD)
        sys.exit("TCP Connection Terminated")
    else:
        sys.exit("UDP Terminated")
    return


# ====================================== User Prompt ====================================== #
try:
    global serverIP, port, clientSocket

    clientSocket = socket.socket()

    if len(sys.argv) != 5:  # expect 4 arguments
        raise ValueError(f"Expected 4 Arguments. Currently have {len(sys.argv) - 1} arguments")

    protocol = sys.argv[1]  # user input protocol (tcp or udp)
    port = int(sys.argv[2])  # user input server port
    debug = int(sys.argv[3])  # user input debug toggle (0: OFF, 1: ON)
    serverIP = sys.argv[4]  # server IP to connect with

    if protocol not in ['tcp', 'udp']:
        raise ValueError("Expected protocol to be either 'tcp' or 'udp'")
    if not (1 <= port <= 9999):
        raise ValueError("Expected port number between 1 to 9999")
    if debug not in [0, 1]:
        raise ValueError("Expected debug option between 0(OFF) or 1(ON)")

    debugMode = bool(debug)  # convert debug to boolean, 0(OFF-False) or 1(ON-True)

except ValueError as e:
    print(f"\nError: {e}")
    raise SystemExit(f"Command Format: python {sys.argv[0]} <protocol> <port> <debug> <IP>\n")

BUFFER_SIZE_BYTES = 2048  # allowed send/receive bytes amount
# current connection information
print(f"\nProtocol: {protocol}, Port: {port}, Debug Mode: {'ON' if debugMode else 'OFF'}, ServerIP: {serverIP}\n")

# ====================================== TCP Socket ====================================== #
if protocol == "tcp":
    tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # tcp: SOCK_STREAM
    tcp_client_socket.connect((serverIP, port))  # connect to server
    clientSocket = tcp_client_socket

    if debugMode:
        print("TCP Connection")  # connection-based tcp protocol successful

# ====================================== UDP Socket ====================================== #
if protocol == "udp":
    udp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP: SOCK_DGRAM
    clientSocket = udp_server_socket

    if debugMode:
        print("UDP connectionless")  # no connection required, only waiting for datagram

# ====================================== User Commands ====================================== #
while True:
    cmd = input("myftp>")  # prompt user
    cmd = cmd.split(" ")  # splot cmd with spaces
    user_cmd = cmd[0]  # get the user command, index 0
    if debugMode:
        print(f"\nUser Command: {user_cmd}")  # print cmd type

    # check if the arguments amount are the expected amount before function call
    if user_cmd == "put":
        if len(cmd) != 2:
            print("Invalid command format. Expected: put <filename>")
        else:
            put(cmd)  # function call
    elif user_cmd == "get":
        if len(cmd) != 2:
            print("Invalid command format. Expected: get <filename>")
        else:
            get(cmd)  # function call
    elif user_cmd == "change":
        if len(cmd) != 3:
            print("Invalid command format. Expected: change <current_filename> <new_filename>")
        else:
            change(cmd)  # function call
    elif user_cmd == "summary":
        if len(cmd) != 2:
            print("Invalid command format. Expected: summary <argument>")
        else:
            summary(cmd)  # function call
    elif user_cmd == "help":
        if len(cmd) != 1:
            print("Invalid command format. Expected: help")
        else:
            help()  # function call
    elif user_cmd == "bye":
        if len(cmd) != 1:
            print("Invalid command format. Expected: bye")
        else:
            bye()  # function call
    else:
        print("Unknown Command, please use 'help'")

    print("")  # new line to make output clearer