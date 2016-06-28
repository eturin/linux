#include <iostream>
#include <sys/types.h>          /* man 2 socket */
#include <sys/socket.h>

#include <netinet/in.h>         /* man 2 bind   */
#include <unistd.h>             /* man 2 close  */
#include <arpa/inet.h>          /* man 3 inet_pton */

int main(){
	int master_s=socket(AF_INET/*ipv4*/,SOCK_STREAM/*tcp*/,IPPROTO_TCP);
	
	sockaddr_in sa;
	sa.sin_family=AF_INET;
	sa.sin_port       =htons(12345);
	inet_pton(AF_INET,"54.213.78.236",&(sa.sin_addr));
	if(connect(master_s,(sockaddr*)&sa,sizeof(sa)))
		std::cout<<"Err connect\n";

	else{
		char Buff[]="Hi,U!";
		send(master_s,Buff,4,MSG_NOSIGNAL);
		recv(master_s,Buff,4+1,MSG_NOSIGNAL);
		shutdown(master_s, SHUT_RDWR);
		close(master_s);
		std::cout<<Buff<<std::endl;
	}

	

	return 0;
}
