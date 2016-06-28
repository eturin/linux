#include<iostream>
#include<boost/asio.hpp>

using namespace boost::asio;

int main(){
	//делаем сервис
	io_service service;

	//делаем экземпляр сокет
	ip::tcp::socket sock(service);
	//подкючаемся
	boost::system::error_code ec;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),80);
	sock.connect(ep,ec);

	//отправляем
	write(sock,buffer("GET /\n",6));

	//получаем
	bool is_ok=true;
	do{
		char buf[128];
		boost::system::error_code ec;
		int len=sock.read_some(buffer(buf,127),ec);
		if(ec==error::eof)
			is_ok=false;
		else if(len>0){
			buf[len]='\0';
			std::cout << buf;
		}
		
	}while(is_ok);

	//отключаемся
	sock.shutdown(sock.shutdown_both);
	sock.close();
	return 0;
}

