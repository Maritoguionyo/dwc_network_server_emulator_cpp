#include "dwc_config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

std::pair<std::string, std::string> get_ip_port() {
    std::string filename = "altwfc.cfg";
    boost::property_tree::ptree config;
    boost::property_tree::ini_parser::read_ini(filename, config);

    std::string section_name = "GameSpyManager";
    std::string ip = config.get<std::string>(section_name + ".IP");
    std::string port = std::to_string(config.get<int>(section_name + ".Port"));

    return std::make_pair(ip, port);
}
