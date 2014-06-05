#include "TcpServer.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <boost/filesystem.hpp>

namespace glPortal {
  TcpServer::TcpServer(boost::asio::io_service& io_service, short port):
    service(io_service), acceptor(io_service, tcp::endpoint(tcp::v4(), port))
  {
    TcpSession* session = new TcpSession(service);
    acceptor.async_accept(session->socket(), boost::bind(&TcpServer::handle_accept, this, session, boost::asio::placeholders::error));
  }

  void TcpServer::handle_accept(TcpSession* session, const boost::system::error_code& error)
  {
    if(!error){
      session->start();
      session = new TcpSession(service);
      acceptor.async_accept(session->socket(),
                             boost::bind(&TcpServer::handle_accept, this, session, boost::asio::placeholders::error));
    } else {
      delete session;
    }
  }
}
