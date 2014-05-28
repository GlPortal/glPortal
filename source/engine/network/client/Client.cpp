#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main()
{
    boost::asio::io_service io_service;

    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), "my ip", "5555");
    udp::resolver::iterator iterator = resolver.resolve(query);

	while(1 < 2)
	{
		s.send_to(boost::asio::buffer("Hello", 5), *iterator);
	}

  return 0;
}
