#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>        
#include <sys/socket.h>   /* man 2 socket */
#include <netinet/in.h>   /* man 2 bind   */
#include <arpa/inet.h>    /* sockaddr_in  */
#include <unistd.h>       /* man 2 close  */
#include <fcntl.h>
#include <sys/select.h>

int set_nonblock(int fd);

int main(){
	//формируем сокет
	int master_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	std::set<int> set_slaves;
	
	sockaddr_in sa;
	sa.sin_family=AF_INET;
	sa.sin_port=htons(1111);
	inet_pton(AF_INET,"0.0.0.0", &(sa.sin_addr));	
	//биндим
	if(bind(master_socket,(sockaddr*)&sa,sizeof(sa)))
		std::cout << "Error on bind.\n";
	else{
		//не блокирующий сокет
		set_nonblock(master_socket);      
		if(listen(master_socket,SOMAXCONN)) //слушаем 
			std::cout << "Error on listen.\n";
		else{
			//повторное использование сокета
		        int optval=1;
			setsockopt(master_socket,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
			//принимаем соединеия
			while(true){
				fd_set set;//1024 бит
                          	FD_ZERO(&set);                                                                                                                 FD_SET(master_socket,&set);   
				for(auto it=set_slaves.begin();it!=set_slaves.end();++it)
					FD_SET(*it,&set);
				
				int max=std::max(master_socket,*std::max_element(set_slaves.begin(),set_slaves.end()));
				//засыпаем
          			select(max+1,&set,NULL,NULL,NULL);
				//проснулись
				for(auto it=set_slaves.begin();it!=set_slaves.end();++it)
					if(FD_ISSET(*it,&set)){
						static char Buf[1024]={0};
						int size = recv(*it,Buf,1024,MSG_NOSIGNAL);
						if(size==0 && errno != EAGAIN){
							//закрываем
			                                shutdown(*it,SHUT_RDWR);
							close(*it);
							set_slaves.erase(it);
						}else if(size!=0){
							send(*it,Buf,size,MSG_NOSIGNAL);
						}
					}
				//на мастере новое подключение
				if(FD_ISSET(master_socket,&set)){
					int slave_socket=accept(master_socket,0,0);
					set_nonblock(slave_socket);
					set_slaves.insert(slave_socket);
				}

			}	
			//закрываем
			shutdown(master_socket,SHUT_RDWR);
			close(master_socket);			
		}
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
