#include <iostream>
#include <sys/types.h>          /* man 2 socket */
#include <sys/socket.h>

#include <netinet/in.h>         /* man 2 bind   */
#include <unistd.h>             /* man 2 close  */

int main(){
	int master_s=socket(AF_INET/*ipv4*/,SOCK_STREAM/*tcp*/,IPPROTO_TCP);
	
	sockaddr_in sa;
	sa.sin_family=AF_INET;
	sa.sin_port       =htons(12345);
	sa.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(master_s,(sockaddr*)&sa,sizeof(sa)))
		std::cout<<"Err in bind\n";
	else if(listen(master_s, SOMAXCONN))
		std::cout<<"Err in listen\n";
	else
		while(true){
			int slave_s=accept(master_s,0,0);
			char Buff[5]={0};
			recv(slave_s,Buff+1,4,MSG_NOSIGNAL);
			std::cout<<Buff+1<<std::endl;
			Buff[0]='+';
			send(slave_s,Buff,5,MSG_NOSIGNAL);
			shutdown(slave_s, SHUT_RDWR);
			close(slave_s);
			std::cout<<Buff<<std::endl;
		}

	

	return 0;
}
