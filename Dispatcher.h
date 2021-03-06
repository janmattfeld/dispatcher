
#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "AbstractDistributor.h"

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>


#define READ 0
#define WRITE 1
#define LOAD 2

class Dispatcher {
private:
    std::queue<int> request_queue;
    std::mutex request_queue_mutex;
    std::condition_variable request_queue_empty;

    char *port;

    int thread_pool_size;
    std::vector<std::thread> parser_thread_pool;

    std::vector<struct Host*> *cluster_nodes;
    std::mutex cluster_nodes_mutex;

    AbstractDistributor *distributor;

    Dispatcher( const Dispatcher& other ); // non construction-copyable
    Dispatcher& operator=( const Dispatcher& ); // non copyable

    void sendToAll(struct HttpRequest *request, int sock);
    void sendNodeInfo(struct HttpRequest *request, int sock);
    
public:
    Dispatcher(char *port, char *settings_file);
    void dispatch_requests(int id);
    void start();
    void shut_down();
    void set_master(const char *url, int port);
    void add_host(const char *url, int port);
    void remove_host(const char *url, int port);
};

#endif  // DISPATCHER_H_
