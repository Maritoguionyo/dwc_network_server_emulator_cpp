#include "StatsPage.h"
#include <boost/bind.hpp>

class InternalStatsServer {
public:
    InternalStatsServer();

    void start();
    std::string get_server_list_json(); // new method

private:
    GameSpyServerDatabase server_manager_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::shared_ptr<boost::asio::io_context> io_context_;

    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       const boost::system::error_code& error);
};
