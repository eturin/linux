#include<iostream>
#include<sys/socket.h>
#include<event2/event.h>
#include<event2/event_struct.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/ioctl.h>
#include <unistd.h>


void cb_master(evutil_socket_t fd, short what, void *ptr);
void cb_slave(evutil_socket_t fd, short what, void *ptr);
int set_non_block(int fd);

int main(){
	//делаем соект
	static evutil_socket_t master_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(-1==master_socket)
		std::cout << "socket():\n"<<errno<<std::endl;
	else{
		//делаем структуру
		struct sockaddr_in sa;
		sa.sin_family=AF_INET;
		sa.sin_port=htons(12345);
		sa.sin_addr.s_addr=htonl(INADDR_ANY);
		//биндим
		if(bind(master_socket,(sockaddr*)&sa,sizeof(sa))==-1)
			std::cout << "bind:\n"<<errno<<std::endl;
		else if(listen(master_socket,SOMAXCONN)==-1)
			std::cout << "listen:\n"<<errno<<std::endl;
		else{
			//повторное использование мастер сокета
			int optval = 1;
			setsockopt(master_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
			//не  блокирующий
			set_non_block(master_socket);
			                                                               
			//делаем ядро
			struct event_base * base=event_base_new();
	
			//регистрируем события
			struct event * evn=event_new(base,master_socket,EV_READ|EV_PERSIST,cb_master,event_self_cbarg());
			event_add(evn,NULL);

			//запускаем цикл
			event_base_dispatch(base);
			
			//очищаем
			event_base_free(base);
			event_free(evn);
			//закрываем
			shutdown(master_socket,SHUT_RDWR);
			close(master_socket);
	
		}
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


void cb_master(evutil_socket_t fd, short what, void *ptr){
	struct event * evn=(event *)ptr;
	evutil_socket_t slave_socket=accept(fd,NULL,NULL);
	if(slave_socket==-1)
		std::cout<<"accept:\n"<<errno<<std::endl;
	else{
		//регистрируем события
		struct event * evn_new=event_new(evn->ev_base,slave_socket,EV_READ|EV_PERSIST,cb_slave,event_self_cbarg());
		event_add(evn_new,NULL);
		std::cout<<"Hi\n";
	}
}

void cb_slave(evutil_socket_t fd, short what, void *ptr){
     struct event * evn=(event *)ptr;
     char buf[256];
     bzero((void*)buf,256);
     int size=recv(fd,buf,255,MSG_NOSIGNAL);
     if(size==0 && errno!=EAGAIN){
	std::cout << "Bay.\n";
	event_del(evn);//снимаем с регистрации
	event_free(evn);
	shutdown(fd,SHUT_RDWR);
	close(fd);
     }else if(size>0){
        std::cout<<buf<<std::endl;
	send(fd,buf,size,MSG_NOSIGNAL);
     }
}
