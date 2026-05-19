#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <boost/asio.hpp>
#include <memory>

extern int global_message_count;

extern std::shared_ptr<
    boost::asio::strand<boost::asio::io_context::executor_type>>
    global_strand;

#endif