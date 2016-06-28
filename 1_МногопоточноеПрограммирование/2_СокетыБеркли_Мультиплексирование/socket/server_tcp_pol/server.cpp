#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netinet/in.h>                                                                                     
#include <unistd.h>               
#include <poll.h>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <cerrno>

const int POLL_SIZE = 2048;
int set_nonblock(int fd);

int main(){
	//мастер соект
	int m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in saddr;
        saddr.sin_family= AF_INET;
	saddr.sin_port  = htons(5555);
	saddr.sin_addr.s_addr= htonl(INADDR_ANY);
	//биндим
	if(-1==bind(m_socket,(sockaddr*)&saddr,sizeof(saddr)))
		std::cout << "Не смогли выполнить bind.\n";
	else if(-1==listen(m_socket,SOMAXCONN))
		std::cout << "Не смогли начать слушать.\n";
	else {
		//повторное использование
		int optval = 1; 
		setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)); 
		//не блокирующий сокет
		set_nonblock(m_socket);

		std::set<int> s_sockets;
		while(true){
			pollfd mpfd[POLL_SIZE];
			//добавляем мастер сокет
			mpfd[0].fd = m_socket;
			mpfd[0].events=POLLIN;
			//добавляем слейвы
			int k=1;
			for(auto it=s_sockets.begin();it!=s_sockets.end();++it){
				mpfd[k].fd=*it;
				mpfd[k++].events=POLLIN;
			}
			
			//засыпаем	
			poll(mpfd,s_sockets.size()+1,-1);

			//проснулись
			if(mpfd[0].revents & POLLIN){
				//принимаем новое соединение
				int s_socket=accept(m_socket,0,0);
				//не блокирующий
				set_nonblock(s_socket);
				s_sockets.insert(s_socket);
			}
			std::vector<int> v_to_del;
			for(int i=1,l=s_sockets.size()+1;i<l;++i){
				if(mpfd[i].revents & POLLIN){
					//собираем сообщение от клиента
					char buf[256];
					std::string msg;
					int cnt=0;
					{
						cnt=recv(mpfd[i].fd,buf,255,MSG_NOSIGNAL);
						if(cnt==0 && errno != EAGAIN){
							//закрываем соединение
							shutdown(mpfd[i].fd,SHUT_RDWR);
							close(mpfd[i].fd);
							v_to_del.push_back(mpfd[i].fd);
						}else{
							buf[cnt]='\0';
							msg+=buf;
						}
						std::cout<<"*";
					}//while(cnt || errno == EAGAIN);
					//печатаем  сообщение на консоль
					if(msg.size())
						std::cout << msg;
				}
			}
			//выбрасываем закрытые соединения
			for(auto it=v_to_del.begin();it!=v_to_del.end();++it)
				s_sockets.erase(std::find(s_sockets.begin(),s_sockets.end(),*it));
		}


		//закрываем мастер сокет
		shutdown(m_socket,SHUT_RDWR);
		close(m_socket);
	}



	return 0;
}

int set_nonblock(int fd){                                                                                                        
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
