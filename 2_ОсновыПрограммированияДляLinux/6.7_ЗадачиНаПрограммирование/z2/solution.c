#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h> //IPPROTO_UDP

#define LEN 5121

int main(int argc, char ** argv){
	//получаем порт
	if(argc<2){
		printf("Используйте утилиту так:\n\t%s 6000\n",*argv);
		return 1;
	}
	int port=atoi(argv[1]);

	//получаем дескриптор сокета
	int sfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sfd==-1){
		perror("Ошибка создания сокета");
		return 2;
	}else{
		//повторное использование сокета 
		int optval = 1;
	       	if(-1==setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)))
			perror("Ошибка режима повторного использования");
	}

	//связываем сокет с сетевым адресом
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	inet_aton("0.0.0.0",&addr.sin_addr);
	addr.sin_port=htons(port);
	int res=bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1){
		perror("Ошибка связывания с сетевым адресом");
		return 3;
	}

	do{
		//читаем из сокета
		struct sockaddr_in addr_client;
		int size;
		char data[LEN];
		ssize_t len=recvfrom(sfd,data,LEN-1,MSG_NOSIGNAL,(struct sockaddr*)&addr_client,&size);
		if(len==-1 && errno!=EAGAIN)
			perror("Ошибка чтения");
		else if(len>0){
			data[len]='\0';
			//проверяем условие выхода
			if(!strcmp(data,"OFF\n"))
				break;
			else
				printf("%s\n",data);
		}
	}while(1);

	//закрываем дескриптор сокета
	close(sfd);

	return 0;
}
