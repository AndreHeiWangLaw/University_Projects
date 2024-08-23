Name: Andre Hei Wang Law
ID: 40175600
Purpose: This is a README instructions on how to run the program
Author: I am the sole author of this file

*"In" is client user input
*"Out" is client user output

[======================= UDP =======================]

== 1. Setup Server ==
In:		cd .\server\
In:		python .\server.py udp 9991 1

*Note: I have implemented error catches
	In:		python .\server.py 
	Out:	Error: Expected 3 Arguments. Currently have 0 arguments
			Command Format: python .\server.py <protocol> <port> <debug> 
			
	In:		python .\server.py udp
	Out:	Error: Expected 3 Arguments. Currently have 1 arguments
			Command Format: python .\server.py <protocol> <port> <debug> 
			
	In:		python .\server.py udp 9991
	Out:	Error: Expected 3 Arguments. Currently have 2 arguments
			Command Format: python .\server.py <protocol> <port> <debug> 
			
	In:		python .\server.py udp 9991 100000
	Out:	Error: Expected debug option between 0(OFF) or 1(ON)
			Command Format: python .\server.py <protocol> <port> <debug>
			
	In:		python .\server.py random 9991 1
	Out:	Error: Expected protocol to be either 'tcp' or 'udp'
			Command Format: python .\server.py <protocol> <port> <debug>
			
	In:		python .\server.py udp 100000 1
	Out:	Error: Expected port number between 1 to 9999
			Command Format: python .\server.py <protocol> <port> <debug>
		

== 2. Setup Client == 
cd .\client\
python .\client.py udp 9991 1 192.168.56.1
*You might need to change "192.168.56.1" to you IP. I used:
"serverIP = socket.gethostbyname(socket.gethostname())" in server.py
to setup the server to my IP. The client will have to manually enter it.
*It also have similar error catch as UDP Section 1. Setup Server


== 3. Put Functionality == 
txt:
	In: 	put num.txt
	Out:	User Command: put
			Correct 'put' or 'change' request
doc:
	In:		put doc.doc
	Out:	User Command: put
			Correct 'put' or 'change' request
png:
	In:		put sn.png
	Out:	User Command: put
			Correct 'put' or 'change' request
pdf:
	In:		put pdf.pdf
	Out:	User Command: put
			Correct 'put' or 'change' request


== 4. Get Functionality == 
txt:
	In:		get num.txt
	Out:	User Command: get
			Correct 'get' request
doc:
	In:		get doc.doc
	Out:	User Command: get
			Correct 'get' request
png:
	In:		get sn.png
	Out:	User Command: get
			Correct 'get' request
pdf:
	In:		get pdf.pdf
	Out:	User Command: get
			Correct 'get' request
			
			
== 5. Summary Functionality == 
In:		summary num.txt
Out:	Statistical Summary for file: num.txt
		Maximum: 14.0, Minimum: 6.0, Average: 10.0


== 6. Change Functionality == 
txt:
	In:		change num.txt nb.txt
	Out:	User Command: change
			Correct 'put' or 'change' request
doc:
	In:		change doc.doc do.doc
	Out:	User Command: change
			Correct 'put' or 'change' request
png:
	In:		change sn.png s.png
	Out:	User Command: change
			Correct 'put' or 'change' request
pdf:
	In:		change pdf.pdf pd.pdf
	Out:	User Command: change
			Correct 'put' or 'change' request

			
== 7. Help Functionality == 
In: 	help
Out:	User Command: help
		bye change get help put summary


== 8. Bye Functionality == 
In: 	bye
Out:	User Command: bye
		TCP Connection Terminated
		
		
		
[======================= TCP =======================]

== 1. Setup Server == 
cd .\server\
python .\server.py tcp 9990 1
*It also have similar error catch as UDP Section 1. Setup Server


== 2. Setup Client == 
cd .\client\
python .\client.py tcp 9990 1 192.168.56.1
*You might need to change "192.168.56.1" to you IP. I used:
"serverIP = socket.gethostbyname(socket.gethostname())" in server.py
to setup the server to my IP. The client will have to manually enter it.
*It also have similar error catch as UDP Section 1. Setup Server


== 3. Put Functionality == 
doc:
	In:		put doc.doc
	Out:	User Command: put
			Correct 'put' or 'change' request
png:
	In:		put sn.png
	Out:	User Command: put
			Correct 'put' or 'change' request
pdf:
	In:		put pdf.pdf
	Out:	User Command: put
			Correct 'put' or 'change' request


== 4. Get Functionality == 
doc:
	In:		get doc.doc
	Out:	User Command: get
			Correct 'get' request
png:
	In:		get sn.png
	Out:	User Command: get
			Correct 'get' request
pdf:
	In:		get pdf.pdf
	Out:	User Command: get
			Correct 'get' request
			
			
== 5. Summary Functionality == 
In:		summary num.txt
Out:	Statistical Summary for file: num.txt
		Maximum: 14.0, Minimum: 6.0, Average: 10.0


== 6. Change Functionality == 
txt:
	In:		change num.txt nb.txt
	Out:	User Command: change
			Correct 'put' or 'change' request
doc:
	In:		change doc.doc do.doc
	Out:	User Command: change
			Correct 'put' or 'change' request
png:
	In:		change sn.png s.png
	Out:	User Command: change
			Correct 'put' or 'change' request
pdf:
	In:		change pdf.pdf pd.pdf
	Out:	User Command: change
			Correct 'put' or 'change' request

			
== 7. Help Functionality == 
In: 	help
Out:	User Command: help
		bye change get help put summary


== 8. Bye Functionality == 
In: 	bye
Out:	User Command: bye
		TCP Connection Terminated