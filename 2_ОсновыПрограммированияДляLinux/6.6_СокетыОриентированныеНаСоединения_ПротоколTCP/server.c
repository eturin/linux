#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define LEN 10

int main(){
	/*получаем дескриптор сокета*/
	int sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sfd==-1){
		perror("Не удалось получить дескриптор сокета");
		return 1;
	}

	/*связываем дескриптор с сетевым адресом*/
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	inet_aton("0.0.0.0",&addr.sin_addr);
	addr.sin_port=ntohs(1234);
	int res=bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1){
		perror("Не удалось связать дескриптор с сетевым адресом");
		return 2;
	}

	/*включаем повторное использование*/
	int optval = 1; 
	if(-1==setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))) 
		perror("Ошибка режима повторного использования");

	/*начинаем прием соединений*/
	res=listen(sfd,SOMAXCONN);
	if(res==-1){
		perror("Не удалось начать прием соединений");
		return 3;
	}

	while(1){
		/*принимаем соединение (формируется slave сокет, через который пойдет обмен)*/
		struct sockaddr_in addr_remote;
		socklen_t adr_len;
		int sfd_slave=accept(sfd,(struct sockaddr*)&addr_remote,&adr_len);
		if(sfd_slave==-1)
			perror("Не удалось принять соединение");
		else{
			/*читаем из сокета определенный размер (!!!блокируемся на это время!!!)*/
			char data[LEN];
			ssize_t len=recv(sfd_slave,data,LEN-1,MSG_NOSIGNAL);
			if(len==-1)
				perror("Ошибка чтения");
			else{
				data[len]='\0';
				printf("%s\n",data);
			}

			/*закрываем вспомогательный дескриптор соединения*/
			shutdown(sfd_slave,SHUT_RDWR);
			close(sfd_slave);
		}
	}

	/*закрываем*/
	close(sfd);

	return 0;
}
