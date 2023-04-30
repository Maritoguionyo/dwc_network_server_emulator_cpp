#include "dwc_config.h"
#include "InternalStatsServer.h"
#include "StatsPage.h"
#include <boost/bind.hpp>

class InternalStatsServer {
public:
    InternalStatsServer();

    void start();

private:
    GameSpyServerDatabase server_manager_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::shared_ptr<boost::asio::io_context> io_context_;

    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       const boost::system::error_code& error);
};

InternalStatsServer::InternalStatsServer() {
    auto address = dwc_config::get_ip_port("GameSpyManager");
    io_context_ = std::make_shared<boost::asio::io_context>();
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(*io_context_);
    acceptor_->open(address.protocol());
    acceptor_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_->bind(address);
    acceptor_->listen();
}


void InternalStatsServer::start() {
    server_manager_.connect();

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_context_);
    acceptor_->async_accept(*socket, [this, socket](const boost::system::error_code& error) {
        if (!error) {
            handle_accept(socket, error);
        }
    });

    io_context_->run();
}

void InternalStatsServer::handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                                        const boost::system::error_code& error) {
    if (!error) {
        auto stats_page = std::make_shared<StatsPage>(*this);

        boost::asio::async_read_until(*socket, stats_page->buffer_, "\r\n",
                                      [this, socket, stats_page](const boost::system::error_code& error,
                                                                 std::size_t bytes_transferred) {
            if (!error) {
                stats_page->handle_request(socket);
            }
        });
    }

    auto new_socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_context_);
    acceptor_->async_accept(*new_socket, [this, new_socket](const boost::system::error_code& error) {
        if (!error) {
            handle_accept(new_socket, error);
        }
    });
}

// Add a new method to get the server list in JSON format
std::string InternalStatsServer::get_server_list_json() {
    auto server_list = server_manager_.get_server_list()->_getvalue();

    // Filter out certain fields before displaying raw data
    for (auto& [game, servers] : server_list) {
        for (auto& server : servers) {
            server.erase("publicip");
            server.erase("__session__");
            server.erase("localip0");
            server.erase("localip1");
        }
    }

    // Convert the server list to JSON and return it as a string
    return json(server_list).dump();
}