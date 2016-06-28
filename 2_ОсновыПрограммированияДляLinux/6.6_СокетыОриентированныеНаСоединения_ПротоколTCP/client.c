#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#define LEN 10

int main(){
	/*получаем дескриптор сокета*/
	int sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sfd==-1){
		perror("Не получен дескриптор сокета");
		return 1;	
	}

	/*связываем сокет с сетевым адресом (с коротым соединяемся)*/
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	inet_aton("127.0.0.1",&addr.sin_addr);
	addr.sin_port=ntohs(1234);
	int res=connect(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1){
		perror("Соединение не удалось");
		close(sfd);
		return 2;
	}

	char data[10*LEN];
	printf("Укажите передаваемые данные:\n");
	scanf("%s",data);
	size_t len=strlen(data);

	/*отправка*/
	ssize_t size=send(sfd,data,len+1,MSG_NOSIGNAL);
	if(size==-1)
		perror("Ошибка отправки");
	else
		printf("Отправлено %lld байт\n",size);

	/*получение*/
	memset(data,'\0',10*LEN);
	while(1){
		size=recv(sfd,data,10*LEN,MSG_NOSIGNAL);
		if(errno)
			perror("Ошибка получения");
		else if(size){
			data[size]='\0';
			if(!strcmp(data,"off\n")) //условие выхода - получен "off\n" 
				break;
			printf(data);
		}
	}

	/*закрываем дескриптор*/
	close(sfd);

	return 0;
}

