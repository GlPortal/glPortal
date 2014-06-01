#include "Server.hpp"
#include "Main.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: glportal_d <port>\n";
      return 1;
    }

    std::cout << "GlPortal Server Listening on Port: ";
    std::cout << argv[1];
    std::cout << "\n";

    boost::asio::io_service io_service;

    glPortal::Server server(io_service, std::atoi(argv[1]));

    io_service.run();
  }
  catch (std::exception& e)
  {
     std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
