#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

void start_accept(boost::shared_ptr<ip::tcp::socket>);
void hendel_accept(boost::shared_ptr<ip::tcp::socket>,const boost::system::error_code &);

ip::tcp::acceptor * p_acc;
io_service * p_service;

int main(){
	//делаем сервис
	p_service=new io_service();
	//делаем акцептор
	ip::tcp::endpoint ep(ip::tcp::v4(),12345);
	p_acc=new ip::tcp::acceptor(*p_service,ep);
	
	//асинхронный акцепт
	boost::shared_ptr<ip::tcp::socket> sptr_sock(new ip::tcp::socket(*p_service));
	start_accept(sptr_sock);
	
	//запускаем
	p_service->run();

	delete p_acc, p_service;
	return 0;
}


void start_accept(boost::shared_ptr<ip::tcp::socket> sptr_sock){
	p_acc->async_accept(*sptr_sock,boost::bind(hendel_accept,sptr_sock,_1));
}

void hendel_accept(boost::shared_ptr<ip::tcp::socket> sptr_sock,const boost::system::error_code & ec){
	if(ec)
		return;
	else{
		//читаем
		char buf[512];
		int len=sptr_sock->read_some(buffer(buf));
		//пишем
		if(len>0){
			write(*sptr_sock,buffer(buf,len));
		}
		sptr_sock->shutdown(sptr_sock->shutdown_both);
		sptr_sock->close();
		boost::shared_ptr<ip::tcp::socket> sptr_sock(new ip::tcp::socket(*p_service));
		start_accept(sptr_sock);
	}
}
