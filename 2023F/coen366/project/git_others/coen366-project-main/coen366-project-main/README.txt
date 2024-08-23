# COEN366 Project

run both the server.py and client.py then interact with client terminal to use. include arguments (debug 1 enables print statements)
server.py [tcp/udp] [ip] [port] [0/1](debug)
client.py [tcp/udp] [ip] [port] [0/1](debug)

Exambles
python .\server.py tcp 192.168.0.1 80 1
python .\client.py tcp 192.168.0.1 80 1

python .\server.py UDP 192.168.0.1 80 1
python .\client.py UDP 192.168.0.1 80 1

The included files are in two locations
Server side
     wonder.pdf
Client Side
     bee_movie.txt
     robotics.png
     rocketry.png

Put and get should make the files appear in the others location. backup of test files found in test folder.
Valid commands include 
put X           X= filname
get X           X= filname
change X Y      X= old_filname Y= new_filename
help
bye

