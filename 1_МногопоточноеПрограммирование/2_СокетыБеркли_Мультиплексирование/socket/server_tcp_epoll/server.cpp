#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/epoll.h>

const int MAX_EVENTS = 256;
int set_non_block(int);

int main(){
	//делаем мастер сокет
	int m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port  =htons(5555);
	saddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//биндим
	if(-1==bind(m_socket,(sockaddr*)&saddr,sizeof(saddr)))
		std::cout << "Не удался bind.\n";
	else if(-1==listen(m_socket,SOMAXCONN))
		std::cout << "Не удался listen.\n";
	else {
		//повторное использование мастер сокета
		int optval = 1;                                                                                                  
                setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));  
		//не  блокирующий
		set_non_block(m_socket);

		int e_poll = epoll_create1(0);
		if(-1==e_poll)
			std::cout << "Не хватает памяти в едре для epoll.\n";
		else{
			epoll_event event;
			event.data.fd=m_socket;
			event.events =EPOLLIN; //EPOLLIN - есть не прочитанные данные
		                               //EPOLLET - есть не новые данные;	
			if(-1==epoll_ctl(e_poll,EPOLL_CTL_ADD,m_socket,&event))
				std::cout << "Не смогли добавить мастер сокет в epoll";
			else
				while(true){
					epoll_event mevents[MAX_EVENTS];
					int n = epoll_wait(e_poll,mevents,MAX_EVENTS,-1);
					for(int i=0;i<n;++i)
						if(mevents[i].data.fd==m_socket){
							//принимаем новое соединение
							sockaddr_in sladdr;
							socklen_t size_of_sladdr=sizeof(sladdr);
							int s_socket=accept(m_socket,(sockaddr*)&sladdr,&size_of_sladdr);
							if(-1==s_socket)
								std::cout << "Не смогли принять новое соединение accept.\n";
							else{
								set_non_block(s_socket);
								//регистрируем в ядре epoll
								event.data.fd=s_socket;
								event.events=EPOLLIN;
								if(-1==epoll_ctl(e_poll,EPOLL_CTL_ADD,s_socket,&event)){
									std::cout << "Не смогли добавить слейв сокет в epoll";
									shutdown(mevents[i].data.fd,SHUT_RDWR);
									close(mevents[i].data.fd);
								}
							}
						}else if(mevents[i].events & EPOLLERR || mevents[i].events & EPOLLHUP){
							//ошибка или разрыв соединения
							shutdown(mevents[i].data.fd,SHUT_RDWR);
							close(mevents[i].data.fd);						
						}else{
							char buf[1024];
							int size_buf=recv(mevents[i].data.fd,buf,1023,MSG_NOSIGNAL);
							if(size_buf==0 && errno != EAGAIN){
								//закрываем соединение
								shutdown(mevents[i].data.fd,SHUT_RDWR);
								close(mevents[i].data.fd);
							}else if(size_buf>0){
								buf[size_buf]='\0';
								std::cout << buf;
								send(mevents[i].data.fd,buf,size_buf,MSG_NOSIGNAL);
							}
						}

				}
		}

		//отключаем мастер сокет
		shutdown(m_socket,SHUT_RDWR);
		close(m_socket);
	}
	return 0;
}

int set_non_block(int fd){
	int flags;  
#if defined(O_NONDLOCK)                                                                                                  
	if(-1==(flags = fcntl(fd,F_GETFL,0)))       
		flags = 0;                                                                                               
		return fcntl(fd,F_SETFL, flags | O_NONBLOCK);
#else
        flags = 1;
        return ioctl(fd,FIONBIO,&flags);
#endif
}
