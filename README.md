# BasicCSocket_onLinux
Socket Assignment for Distributed System at Tampere University of Technology, Finland. For details, refer to http://www.cs.tut.fi/~hajap/assignments/socket/.



#How to run project:
Run make command as below from /src:  <br />
<i>make sock_server  <br />
make sock_client </i>  <br />
This will generate a sock_server and a sock_client file.
From terminal, run first <i> ./sock_server </i> to open server socket.  <br />
Then, from another terminal, navigate to /src and run <i>sock_client</i> to run client.
Default login is:  <br />
debug: 111 <br />


#Requirements:
UNIX environment.  <br />
OpenSSL installed  <br />

#Design concept:
1.Client will initialize a TCP connection to the server and provide a username  U<br />

2.Server  :
-Queries user database and gets password P1 of username U (if user is unknown the connection is terminated) <br />
-Creates a random number R, which is sent to the client <br />

3.Client:
-Asks an password P2 from the user <br />
-Concatenates the password and the random number (P2+R) and calculates from this data an SHA256-hash value H1. <br />
-Value H1 is sent to the server <br />

4.Server: 
-Calculates SHA256-hash( P1+R ) value H2. <br />
-Compares values H1 ja H2. <br />
-If values are the same, then password is correct and connection is accepted. ELSE the connection is terminated <br />

5.Server and Client start sending 'ping' . Transmission is carried out using UDP Protocol with HMAC verification using common
key namely H1 and H2 as above.



