#pragma once

#include <boost/asio.hpp>
#include <string>
#include <memory>

class InternalStatsServer;

class StatsPage {
public:
    StatsPage(InternalStatsServer& stats);

    void handle_request(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

private:
    std::string get_server_list_json();
    std::string get_formatted_time();

    InternalStatsServer& stats_;

    const std::string header_ = R"(<html>
    <table border='1'>
        <tr>
            <td>Game ID</td><td># Players</td>
        </tr>)";

    const std::string row_ = R"(
        <tr>
            <td>%s</td>
            <td><center>%d</center></td>
        </tr>)";

    const std::string footer_ = "</table><br><i>Last updated: %s</i><br></html>";
};
