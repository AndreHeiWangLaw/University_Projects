# Andre Hei Wang Law
# 4017 5600
# Socket Programming Assignment

import socket

def create_server():
    # Create a socket to listen on port 12000
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 12000))
    server_socket.listen(1)  # Listen for incoming connections

    print("Server is listening on port 12000...")

    while True:
        # Accept a connection from a client
        client_socket, client_address = server_socket.accept()

        # Read the HTTP request from the client
        request = client_socket.recv(1024).decode('utf-8')

        # Parse the request to get the requested filename
        filename = request.split(' ')[1]

        # Remove the leading '/' character
        filename = filename[1:]

        try:
            # Open and read the requested file
            with open(filename, 'r') as file:
                content = file.read()
                response = "HTTP/1.0 200 OK\n\n" + content
        except FileNotFoundError:
            # If the file doesn't exist, return a 404 Not Found response
            response = "HTTP/1.0 404 Not Found\n\nFile not found"
        except Exception as e:
            # If any other error occurs, return a 500 Internal Server Error response
            response = "HTTP/1.0 500 Internal Server Error\n\n" + str(e)

        # Send the HTTP response to the client
        client_socket.sendall(response.encode('utf-8'))

        # Close the connection
        client_socket.close()

if __name__ == "__main__":
    create_server()
