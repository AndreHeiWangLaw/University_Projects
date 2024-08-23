# Andre Hei Wang Law
# 4017 5600
# Socket Programming Assignment 1

# Code based on "Module 2" Moodle slides, page.104

# URL link: http://localhost:12000/coen366.html

from socket import *

serverPort = 12000

# create TCP welcoming socket
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))

# server begins listening for incoming TCP requests
serverSocket.listen(1)
print("The server is ready to receive")

# loop forever
while True:
    # server waits on accept() for incoming
    # requests, new socket created on return
    connectionSocket, addr = serverSocket.accept()

    # read bytes from socket (but not address as in UDP)
    request = connectionSocket.recv(1024).decode()

    # well-formed request (return error otherwise)
    try:
        with open('coen366.html', 'r') as htmlFile:
            htmlContent = htmlFile.read()
            response = "HTTP/1.1 200 OK\r\n\r\n" + htmlContent
    except FileNotFoundError:   # error when file doesn't exist
        response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found"
    except PermissionError:     # error when permissions are set properly
        response = "HTTP/1.1 403 Forbidden\r\n\r\nPermission denied"

    # send HTTP response to the client (browser)
    connectionSocket.send(response.encode())

    # close the connection
    connectionSocket.close()
