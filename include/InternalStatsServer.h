#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "dwc_config.h"

class GameSpyServerDatabase {
public:
    GameSpyServerDatabase();

    std::vector<std::string> get_server_list();

private:
    typedef boost::interprocess::allocator<std::string, boost::interprocess::managed_shared_memory::segment_manager> ShmemAllocator;
    typedef boost::interprocess::vector<std::string, ShmemAllocator> ShmemVector;

    boost::interprocess::named_mutex mutex_;
    boost::interprocess::managed_shared_memory segment_;
    ShmemVector* servers_;
};

class InternalStatsServer {
public:
    InternalStatsServer();

    void start();
    std::string get_server_list_json();

private:
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::shared_ptr<boost::asio::io_context> io_context_;

    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       const boost::system::error_code& error);

    GameSpyServerDatabase* server_manager_;
};
