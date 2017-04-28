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
	
	host_sockid = socket(PF_INET, SOCK_STREAM, 0);
	if(host_sockid == -1)
		DieWithError("[-] socket() failed.\n");

	printf("[+] Created socket for server with fd %d.\n", host_sockid);
	
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(1337);
	hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	status = bind(host_sockid, (struct sockaddr*) &hostaddr, sizeof(hostaddr));
	if(status == -1) DieWithError("[-] bind() failed.\n");	
	printf("[+] Binded successfully.\n");
	
	status = listen(host_sockid, 5);
	if(status == -1) DieWithError("listen() failed.\n");
	
	printf("[+] Currently listening...\n");
	
	while (1)
	{
		clientaddr_size = sizeof(clientaddr);
		client_sockid = accept(host_sockid, (struct sockaddr*) &clientaddr, &clientaddr_size);
		if(status == -1) DieWithError("listen() failed\n");
	
		printf("[+] Connection established successfully! Client ip: %s, client port: %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		while(1)
		{
			num_bytes = recv(client_sockid, buf, 1024, 0);
			if(num_bytes == -1) DieWithError("recv() failed\n");
			if(!num_bytes) break;
			
			printf("[+] Received %d bytes, containing %.*s\n", num_bytes, num_bytes-1, buf);
			
			
			num_bytes = send(client_sockid, buf, num_bytes, 0);
			if(num_bytes == -1) DieWithError("recv() failed\n");
			if(!num_bytes) break;
			printf("[+] Sent %d bytes, containing %.*s\n", num_bytes, num_bytes-1,  buf);
		}
		status  = close(host_sockid);
		if(status == -1) DieWithError("[-] close() failed.\n");
		
		printf("[+] Closed connection successfully.\n");
	}
	return 0;
}
