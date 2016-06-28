#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

void client_session(boost::shared_ptr<ip::tcp::socket>);

int main(){
	//делаем сервис
	io_service service;

	//делаем аксцептор
	ip::tcp::endpoint ep(ip::tcp::v4(),12345);
	ip::tcp::acceptor acc(service,ep);

	while(true){
		//умный указатель shared на сокет
		boost::shared_ptr<ip::tcp::socket> sptr_sock(new ip::tcp::socket(service));
		acc.accept(*sptr_sock);
		boost::thread(boost::bind(client_session,sptr_sock));

	}


	return 0;
}

void client_session(boost::shared_ptr<ip::tcp::socket> sptr_sock){
	while(true){
		char buf[512];
		int len=sptr_sock->read_some(buffer(buf));
		if(len>0){
			write(*sptr_sock,buffer(buf,len));
			write(*sptr_sock,buffer("\nOK!\n",5));
		}
	}
	sptr_sock->shutdown(sptr_sock->shutdown_both);
	sptr_sock->close();
}
