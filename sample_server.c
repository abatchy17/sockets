#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void DieWithError(char* msg)
{
		printf("%s\n", msg);
		exit (1);
}

int main()
{
	int host_sockid, client_sockid, status, num_bytes, clientaddr_size;
	
	char buf[1024];
	
	struct sockaddr_in hostaddr, clientaddr;
	
	// Create socket of domain PF_INET, using TCP
	// socket(family, type, protocol);
	
	// family: PF_INET for IPv4 protocol
	// type: SOCK_STREAM for a TCP socket
	// protocol: SOCK_STREAM has only one protocol, thus using zero (first enum element)
	host_sockid = socket(PF_INET, SOCK_STREAM, 0);
	if(host_sockid == -1)
		DieWithError("socket() failed\n");

	printf("Created socket for server with fd %d\n", host_sockid);
	
	// Initialize the sockaddr_in struct
	// sin_family: AF_INET for IPv4 protocol
	// sin_port: 1337 !111!!!
	// sin_addr.s_addr: Any interface
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(1337);
	hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// Assign address to sockid using bind
	// bind(sockid, &addrport, size);
	
	// sockid: created by socket()
	// addrport: address and port to which the socket will bind
	// size: size of addrport struct
	status = bind(host_sockid, (struct sockaddr*) &hostaddr, sizeof(hostaddr));
	if(status == -1)
		DieWithError("bind() failed\n");
	
	printf("Binded successfully!\n");
		
	// Listen for connections, should be used for a server ONLY
	// listen(sockid, queuelimit);
	
	// sockid: created by socket()
	// queuelimit: max number of participants waiting for a connection
	status = listen(host_sockid, 5);
	if(status == -1)
		DieWithError("listen() failed\n");
		
	// Establish a connection initiated by client
	// client_sockid = accept(sockid, &clientAddr, &addrLen);
	
	// client_sockid: new socket
	// sockid: existing socket
	// clientAddr: struct defining the client, filled in upon return
	// addrLen: size of clientAddr
	clientaddr_size = sizeof(clientaddr);
	client_sockid = accept(host_sockid, (struct sockaddr*) &clientaddr, &clientaddr_size);
	if(status == -1)
		DieWithError("listen() failed\n");

	printf("Connection established successfully! Client socket id is %d\n", client_sockid);
	
	// Receive data
	// count = recv(sockid, recvBuf, bufLen, flags)
	num_bytes = recv(client_sockid, buf, 1024, 0);
	printf("Received %d bytes, containing %s\n", num_bytes, buf);
	
	
	// Close a socket
	// close(sockid);
	status  = close(host_sockid);
	
	return 0;
}
