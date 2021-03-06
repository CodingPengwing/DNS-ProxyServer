# DNS Proxy Server #

This is a solution to project 2 of COMP30023 Computer Systems unit, University of Melbourne. The 
specifications for this project can be found in the repo.

Description:  
This is a proxy between a client and an upstream DNS server. It receives DNS messages in TCP format, 
parses them, then forwards the message on to the upstream server. Responses from the server are parsed, 
then passed back to the client. The program logs all of these interactions, where the first 'question' 
in a query is logged, and the first 'answer' in a response is logged. All of the logs are saved in a 
file called 'dns_svr.log'. The program also has a cache functionality, where the last 5 website 
addresses queried and their corresponding responses are saved, and returned straight away to later 
queries that ask for the same address. This proxy server uses multithreading to process concurrent 
queries.

_Note_: all interactions between client and proxy, proxy and upstream server is done using IPv4 addresses. 
This will, of course, only run when there is an internet connection.

**Set up instructions**:  
The program takes in 2 inputs, an IP address of the upstream server and the port number of that server 
to connect to. We will use port 53 by default since we are processing DNS queries.  
(Explanation for why we take in these parameters: To establish a connection with the upstream server,
we need to create a 5-tuple socket that includes the server's IP address and port. This is different 
from the other 5-tuple socket we will need in the program, which will used to listen for connections
from clients.)

Prior to being able to run the server locally, the user needs to find out the IP address of the upstream 
server. The user can use the address of the local system's Domain Name System (DNS) resolver, this can 
be found by going to terminal and checking the system's resolv.conf file (UNIX systems):
- cd ~
- cat /etc/resolv.conf  
You should be given one or more IP addresses. Pick any that is of IPv4 format. (ie. 170.27.1.1)

Go to Makefile and change 'SERVER_ADDRESS' at the top of the file to be the IP address you found in 
resolv.conf.

**Run instructions:**  
Now you can run the server. In terminal, navigate to the folder containing this program, then run the 
following commands
- make
- make run

**Test instructions:**  
Open a new terminal window, unrelated to the one used for running the proxy. Run the following command:
- dig +tcp -p 8053 @127.0.0.1 <some_website_address>  
(address 127.0.0.1 indicates this is the local device, proxy will be set up at port 8053 by default)  
To compare the result to google's DNS server, run the following command:
- dig +tcp -p 53 @8.8.8.8 <some_website_address>  
(query straight to google's server 8.8.8.8, at port 53)  

To stop running the proxy server: press ctrl+C

**Clean up instructions**:
- make clean
