#include <iostream>
#include <sys/types.h>          /* man 2 socket */
#include <sys/socket.h>

#include <netinet/in.h>         /* man 2 bind   */
#include <unistd.h>             /* man 2 close  */
#include <sys/ioctl.h>          /* FIOBIO */
int set_nonblock(int fd);
int main(){
	int master_s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in sa;
	sa.sin_family     =AF_INET;
	sa.sin_port       =htons(6666);
	sa.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(master_s,(sockaddr*)&sa,sizeof(sa)))
		std::cout<<"Err in bind\n";
	else if(listen(master_s, SOMAXCONN))
		std::cout<<"Err in listen\n";
	else{
		int optval = 1;
		setsockopt(master_s,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
		while(true){
			int slave_s=accept(master_s,0,0);
			//set_nonblock(slave_s);
			char Buff[5]={0};
			int cnt;
			do{
				cnt=recv(slave_s,Buff,1,MSG_NOSIGNAL);
				if(cnt>0){
					std::cout<<Buff;
					send(slave_s,Buff,1,MSG_NOSIGNAL);
				}
			}while(cnt>0);
			shutdown(slave_s, SHUT_RDWR);
			close(slave_s);
		}
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

