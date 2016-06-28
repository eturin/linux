#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

void conn_hendled(const boost::system::error_code &);

int main(){
	//делаем сервис
	io_service service;
	//делаем сокет
	ip::tcp::socket sock(service);
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),80);
	sock.async_connect(ep,conn_hendled);
	service.run();

	std::cout << "Пока.\n";
	return 0;
}

void conn_hendled(const boost::system::error_code & ec){
	std::cout << "Упс!\n";
}
