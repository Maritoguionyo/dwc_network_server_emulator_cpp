#include <iostream>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include "GameSpyBackendServer.h"
#include "GameSpyQRServer.h"
#include "GameSpyProfileServer.h"
#include "GameSpyPlayerSearchServer.h"
#include "GameSpyGamestatsServer.h"
#include "GameSpyNatNegServer.h"
#include "NasServer.h"
#include "Dls1Server.h"
#include "InternalStatsServer.h"
#include "AdminPageServer.h"
#include "StorageServer.h"
#include "GameStatsServer.h"
#include "RegPageServer.h"

int main() {
    boost::asio::io_service io_service;

    GameSpyBackendServer backend_server(io_service);
    GameSpyQRServer qr_server(io_service);
    GameSpyProfileServer profile_server(io_service);
    GameSpyPlayerSearchServer player_search_server(io_service);
    GameSpyGamestatsServer gamestats_server(io_service);
    GameSpyNatNegServer natneg_server(io_service);
    NasServer nas_server(io_service);
    Dls1Server dls1_server(io_service);
    InternalStatsServer internal_stats_server(io_service);
    AdminPageServer admin_page_server(io_service);
    StorageServer storage_server(io_service);
    GameStatsServer game_stats_server(io_service);
    RegPageServer reg_page_server(io_service);

    std::vector<std::thread> threads;

    threads.emplace_back([&backend_server]() { backend_server.start(); });
    threads.emplace_back([&qr_server]() { qr_server.start(); });
    threads.emplace_back([&profile_server]() { profile_server.start(); });
    threads.emplace_back([&player_search_server]() { player_search_server.start(); });
    threads.emplace_back([&gamestats_server]() { gamestats_server.start(); });
    threads.emplace_back([&natneg_server]() { natneg_server.start(); });
    threads.emplace_back([&nas_server]() { nas_server.start(); });
    threads.emplace_back([&dls1_server]() { dls1_server.start(); });
    threads.emplace_back([&internal_stats_server]() { internal_stats_server.start(); });
    threads.emplace_back([&admin_page_server]() { admin_page_server.start(); });
    threads.emplace_back([&storage_server]() { storage_server.start(); });
    threads.emplace_back([&game_stats_server]() { game_stats_server.start(); });
    threads.emplace_back([&reg_page_server]() { reg_page_server.start(); });

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
