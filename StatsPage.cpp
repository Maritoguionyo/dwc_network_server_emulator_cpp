#include "StatsPage.h"
#include "InternalStatsServer.h"
#include <boost/format.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

StatsPage::StatsPage(InternalStatsServer& stats) : stats_(stats) {}

void StatsPage::handle_request(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    // Read the request
    boost::asio::streambuf buffer;
    boost::asio::read_until(*socket, buffer, "\r\n");

    // Extract the request type (GET or POST) and resource URI from the request line
    std::istream request_stream(&buffer);
    std::string request_line;
    std::getline(request_stream, request_line);
    std::string request_type = request_line.substr(0, request_line.find(' '));
    std::string resource_uri = request_line.substr(request_type.length() + 1,
        request_line.rfind(' ') - (request_type.length() + 1));

    // Handle the request
    std::string response;

    if (resource_uri == "/json") {
        response = get_server_list_json();
    } else {
        response = header_;

        auto server_list = stats_.get_server_list();
        if (server_list) {
            for (const auto& [game, servers] : *server_list) {
                if (servers.empty()) {
                    continue;
                }
                response += boost::str(boost::format(row_) % game % servers.size());
            }
        }
        response += boost::str(boost::format(footer_) % get_formatted_time());
    }

    // Send the response
    boost::asio::write(*socket, boost::asio::buffer(response));
}

std::string StatsPage::get_server_list_json() {
    auto server_list = stats_.get_server_list();
    if (!server_list) {
        return "{}";
    }

    // Filter out certain fields before displaying raw data
    for (auto& [game, servers] : *server_list) {
        for (auto& server : servers) {
            server.erase("publicip");
            server.erase("__session__");
            server.erase("localip0");
            server.erase("localip1");
        }
    }

    json j = *server_list;
    return j.dump();
}

std::string StatsPage::get_formatted_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%F %T %Z");
    return ss.str();
}
