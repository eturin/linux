
int main(){
	//domains
	//AF_INET  ip_v4
	//AF_INET6 ip_v6
	//AF_UNIX  unix
	//types
	//SOCK_STREAM tcp
	//SOCK_DGREAM udp
	//protocols
	//0           auto
	//IPPROTO_TCP tcp
	//IPPROTO_UDP udp
	//socket(domain,type,protocol)
	int s=std::socket(AF_INET,SOCK_DGREAM,0);
	
	//bind(s,sockaddr* sa, sizeof(sa))
	//для ip_v6 sockaddr_in6
	//для ip_v4
	std::sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port   = htons(12345);
	//INADDR_LOOPBACK 127.0.0.1
	//INADDR_ANY      0.0.0.0
	//int ip=inet_addr("10.0.0.1");                          //устаревшая
	//       inet_pton(AF_INET,"10.10.10.1",&(sa.sin_addr)); //новая
	sa.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
	//для unix
	//sockaddr_un sa;
	//sa.sun_family = AF_UNIX;
	//std::strcpy(sa.sun_path,"/tmp/a.sock");
	std::bind(s,sa,sizeof(sa));

	//использовать надо
	//int flags = MSG_NOSIGNAL; --игнорировангие SIGPIPE
	//size_t recvfrom(int s1,void * buf,size_t count, int flags, socaddr * from, socklen_t * fromlen);
	//size_t sendto(int s,const void * buf,size_t count,int flags,const sockaddr * to, Socklen_t tolen);
	}
	//576 байт - по UDP придет не делимо
	
	close(s);
	return 0;
}
