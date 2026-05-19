#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client> {
public:
  Client(boost::asio::io_context &io_context);

  void start(const tcp::resolver::results_type &endpoints);

  void send_message(const std::string &message);

private:
  void do_connect(const tcp::resolver::results_type &endpoints);

  void do_read();

private:
  tcp::socket socket_;

  bool connected_ = false;

  enum { max_length = 1024 };

  char data_[max_length];
};

#endif