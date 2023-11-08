#include <string>
#include <memory>
#include <thread>
#include <future>

#include "block.h"

#include "clsocket/src/ActiveSocket.h"
#include "clsocket/src/PassiveSocket.h"

typedef std::shared_ptr<CActiveSocket> CActiveSocketPtr;
typedef std::shared_ptr<CPassiveSocket> CPassiveSocketPtr;

// ----- Abstract class -----
class InterfaceServer {
public:
    virtual ~InterfaceServer() {};

    virtual void Start() = 0;

    virtual void Stop() = 0;

private:

    virtual bool receive_msg(CActiveSocketPtr socket_ptr, std::string& dst, size_t size) = 0;

    virtual bool send_msg(CActiveSocketPtr socket_ptr, const uint8_t* src, size_t size) = 0;
};
// ----- Abstract class -----


class Server : public InterfaceServer {
public:

    Server();

    ~Server();

/**
 * Start the server
*/
    void Start() override;

/**
 * Stop the server
*/
    void Stop() override;

private:

/**
 * Start listen 
*/
    void start_listen();

/**
 * @param socket_ptr - client
 * @param dst - destination string
 * @param size - size of destination string
 * @return true if success
*/
    bool receive_msg(CActiveSocketPtr socket_ptr, std::string& dst, size_t size) override;

/**
 * @param socket_ptr - client
 * @param src - source array
 * @param size - array size
 * @return true if success
*/
    bool send_msg(CActiveSocketPtr socket_ptr, const uint8_t* src, size_t size) override;

private:
    CPassiveSocketPtr m_socket; // passive socket
    CActiveSocketPtr m_client;  // active socket

    bool m_is_started = false; 
    std::future<void> th;

    block_device_ptr m_block_device;
};

typedef std::shared_ptr<Server> server_ptr;