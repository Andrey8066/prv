#include "GlobalState.hpp"

int global_message_count = 0;

std::shared_ptr<boost::asio::strand<boost::asio::io_context::executor_type>>
    global_strand;