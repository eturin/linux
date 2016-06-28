#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/event.h>

int set_non_block(int);

int main(){
	int m_socket = socket(AF_INET,SOCK_STREAM,0);
        sockaddr_in saddr;
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(19345);
	saddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	if(-1==bind(m_socket,(sockaddr*)&saddr,sizeof(saddr)))
		std::cout <<errno << "\nError on bind.\n";
	else if(-1==listen(m_socket,SOMAXCONN))
		std::cout << "Error on listen.\n";
	else{
		int optval = 1;                                                 
                setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
		set_non_block(m_socket);
                
		int kq=kqueue();
		if(-1==kq)
			std::cout << "Error on kqueue.\n";
		else{
			struct kevent kEv;
			bzero(&kEv,sizeof(kEv));
			EV_SET(&kEv,m_socket,EVFILT_READ,EV_ADD,0,0,0); 	
			if(-1==kevent(kq,&kEv,1,NULL,0,NULL))
				std::cout << "Error on kevent:\n"<<errno<<std::endl;
			else while(true) {
				bzero(&kEv,sizeof(kEv));
				kevent(kq,NULL,0,&kEv,1,NULL);
				
				if(kEv.filter==EVFILT_READ){
					if(kEv.ident==m_socket){
						int s_socket=accept(m_socket,0,0);
						if(-1==s_socket)
							std::cout << "Error on accept:\n"<<errno<<std::endl;
						else{
							set_non_block(s_socket);
							bzero(&kEv,sizeof(kEv));
							EV_SET(&kEv,s_socket,EVFILT_READ,EV_ADD,0,0,0);
							if(-1==kevent(kq,&kEv,1,NULL,0,NULL))
								std::cout << "Error on kevent for slave:\n"<<errno<<std::endl;
						}
					}else{
						char buff[256];
						int cnt=recv(kEv.ident,buff,255,MSG_NOSIGNAL);
						if(cnt<=0){
							shutdown(kEv.ident,SHUT_RDWR);
							close(kEv.ident);
						}else if(cnt>0){
							buff[cnt]='\0';
							std::cout << buff<<std::endl;
							send(kEv.ident,buff,cnt,MSG_NOSIGNAL);
						}
					}
				}
			}

		}
		
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
