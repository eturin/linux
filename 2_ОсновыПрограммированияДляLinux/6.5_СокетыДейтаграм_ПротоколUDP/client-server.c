#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h> //IPPROTO_TCP IPPROTO_UDP

#define LEN 256

int main(){
	//создаем дескриптор сокета (некую структуру в ядре)
	int s_fd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(-1==s_fd){
		perror("Ошибка создания сокета");
		return 1;
	}

	//связываем дескриптор с сетевым адресом
	struct sockaddr_in addr_local;
	addr_local.sin_family=AF_INET;
	inet_aton("0.0.0.0",&addr_local.sin_addr);
	addr_local.sin_port=htons(4000);
	int res=bind(s_fd,(struct sockaddr*)&addr_local,sizeof(addr_local));//после успеха уже можно писать в этот порт как на сервер
	if(-1==res){
		perror("Не удалось связать socket с сетевым адресом");
		return 2;
	}

	//формируем сетевой адрес назначения (фрагмент кода клиента)
	struct sockaddr_in addr_remout;
	addr_remout.sin_family=AF_INET;
	inet_aton("54.213.78.236",&addr_remout.sin_addr);
	addr_remout.sin_port=htons(3000);

	//запись в сокет UDP
	char data[LEN];
	strcpy(data,"Привет, Чубз!");
	size_t len=strlen(data)+1;
	ssize_t l=sendto(s_fd,data,len,MSG_NOSIGNAL,(struct sockaddr*)&addr_remout,sizeof(addr_remout));
	if(-1==l)
		perror("Ошибка отправки");
	
	//чтение из сокет UDP (фрагмент кода сервера)
	struct sockaddr_in addr_client;
	socklen_t size;
	l=recvfrom(s_fd,data,LEN-1,MSG_NOSIGNAL,(struct sockaddr*)&addr_client,&size);
	if(-1==l)
		perror("Ошибка чтения из сокета");
	else{
		data[l]='\0';
		char name[LEN];
		inet_ntop(addr_remout.sin_family,&addr_remout.sin_addr,name,LEN);
		printf("%s:%d:\n%s\n",name,ntohs(addr_client.sin_port),data);

		//отправляем назад
		l=sendto(s_fd,data,l,MSG_NOSIGNAL,(struct sockaddr*)&addr_client,sizeof(addr_client));
		if(-1==l)
			perror("Ошибка повторной отправки");
	}
	
	//закрываем
	close(s_fd);

	return 0;
}
