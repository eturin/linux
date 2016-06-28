#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

#define LEN 10

struct Client{
	int sfd;
	char * data;
	size_t len;
	bool is_read;
	struct Client *next;
};

int set_nonblock(int);
int cmp(const void *a, const void *b) { //для сортировки в обратном порядке
	return *(char*)b - *(char*)a; 
}

int main(int argc, char ** argv){
	/*разбираем аргумент*/
	if(argc<2){
		printf("Используйте:\n\t%s 1355\n",*argv);
		return 1;
	}

	int port=atoi(*(argv+1));
	if(port<=0){
		printf("Номер порта должен быть больше нуля.\n");
		return 2;
	}

	pid_t pid=fork();
	if(pid==-1){
		perror("Не удалось породить потомка");
		return 3;
	}

	if(!pid){
		/*потомок*/
		chdir("/");
		pid=setsid();
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		/*получение дескриптора сокета*/
		int sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sfd==-1)
			return 1;
		/*связываем дескриптор с сетевым адресом*/
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		inet_aton("127.0.0.1",&addr.sin_addr);
		addr.sin_port=ntohs(port);
		int res=bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
		if(res==-1)
			return 2;
		/*включаем повторное использование*/ 
		int optval = 1; 
		setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
		/*делаем сокет не блокирующим*/
		res=set_nonblock(sfd);
		if(res==-1)
			return 3;
		/*запускаем прослушку (в этот момент сетевой адрес начинает использоваться)*/
		res=listen(sfd,SOMAXCONN);
		if(res!=0)
			return 4;
		/*мультиплексирование*/
		fd_set set_r,set_w;
		struct Client *root=NULL, *tail=root;
		bool is_repeat=true;
		while(is_repeat){
			/*инициализация*/
			FD_ZERO(&set_r);
			FD_ZERO(&set_w);
			/*регистрируем мастер сокет*/
			FD_SET(sfd,&set_r);
			/*регистрируем слейв сокеты*/
			int max=sfd;
			struct Client * cur=root;
			while(cur!=NULL){
				if(cur->is_read)
					FD_SET(cur->sfd,&set_r);
				else
					FD_SET(cur->sfd,&set_w);
				if(max<cur->sfd)
					max=cur->sfd;
				cur=cur->next;
			}
			/*засыпаем*/
			select(max+1,&set_r,&set_w,NULL,NULL);//эта команда сбрасывает не готовые биты поля, поэтому нужно регистрировать поновой
			/*проверяем слейв сокеты*/
			struct Client * last=NULL;
			cur=root;
			while(cur!=NULL){
				if(FD_ISSET(cur->sfd,&set_r)){
					//получение очередного форагмента сообщения
					char data[LEN];
					ssize_t len=recv(cur->sfd,data,LEN,MSG_NOSIGNAL);
					if(len==0 && errno != EAGAIN){
						//разрыв соединения 
						shutdown(cur->sfd,SHUT_RDWR);
						close(cur->sfd);
						free(cur->data);
						if(last!=NULL)
							last->next=cur->next;
						else
							root=tail=NULL;
						if(cur==tail)
							tail=last;
						free(cur);
						cur=last;
					}else if(len>0 /*&& realloc(cur->data,cur->len+len+sizeof(char))!=NULL*/){
						strncpy(cur->data+cur->len/sizeof(char),data,len);
						cur->len+=len;
						cur->data[cur->len]='\0';
						//условие выхода
						if(!strcmp(cur->data,"OFF"))
							is_repeat=false;
						if(data[len-1]=='\0'){
							cur->is_read=false;//все данные получены, нужно отправлять назад	
							//сортируем
							qsort(cur->data,cur->len-sizeof(char),sizeof(char),cmp);
						}
					}
				}else if(FD_ISSET(cur->sfd,&set_w)){
					//отправляем
					ssize_t len=send(cur->sfd,cur->data,cur->len,MSG_NOSIGNAL);
					if(len==0 && errno!=EAGAIN){
						//разрыв соединения
						shutdown(cur->sfd,SHUT_RDWR);
						close(cur->sfd);
						free(cur->data);
						if(last!=NULL)
							last->next=cur->next;
						else
							root=tail=NULL;
						if(cur==tail)
							tail=last;
						free(cur);
						cur=last;
					} else if(len>0){
						//успешно отправлено
						cur->len=0;
						//void * tmp=realloc(cur->data,sizeof(char));
						cur->is_read=true;
					}
				}
				last=cur;
				if(cur!=NULL)
					cur=cur->next;
			}
			/*проверяем мастер сокет*/
			if(FD_ISSET(sfd,&set_r)){
				/*принимаем новое соединение*/
				int sfd_slave=accept(sfd,NULL,0);
				if(sfd_slave!=-1){
					set_nonblock(sfd_slave);
					struct Client * cur=(struct Client*)malloc(sizeof(struct Client));
					cur->sfd=sfd_slave;
					cur->next=NULL;
					cur->is_read=true;
					cur->len=0;
					cur->data=(char*)malloc(4026*sizeof(char));
					cur->data[0]='\0';
					if(root==NULL)
						root=tail=cur;
					else
						tail=tail->next=cur;
				}
			}
		}

		/*освобождаем ресурсы*/
		struct Client * cur=root;
		while(cur!=NULL){
			free(cur->data);
			shutdown(cur->sfd,SHUT_RDWR);
			close(cur->sfd);
			struct Client *tmp=cur;
			cur=cur->next;
			free(tmp);
		}
		root=tail=NULL;

		/*закрываем дескриптор*/
		shutdown(sfd,SHUT_RDWR);
		close(sfd);
	}


	return 0;
}

int set_nonblock(int fd){ 
	int flags; 
#if defined(O_NONBLOCK) 
	if(-1==(flags=fcntl(fd,F_GETFL,0))) 
		flags=0; 
	return fcntl(fd,F_SETFL,flags | O_NONBLOCK); 
#else 
	flags=1; 
	return ioctl(fd,FIOBIO,&flags); 
#endif 
}
