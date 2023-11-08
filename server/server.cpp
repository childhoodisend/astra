#include "server.h"

#include "clsocket/src/ActiveSocket.h"

#include <iostream>
#include <string>
#include <sstream>

static const size_t SERVER_PORT = 4000;
static const char* SERVER_HOST = "";
static const size_t MAX_HASH = 40;
static const size_t MAX_BUFFER_SIZE = 256;


Server::Server() {
    std::cout << "Server::Server()" << std::endl;

    m_block_device = std::make_shared<BlockDevice>("/tmp");

    m_socket = std::make_shared<CPassiveSocket>();
    m_socket->Initialize();
    m_socket->Listen(SERVER_HOST, SERVER_PORT);
}

Server::~Server() {
    std::cout << "Server::~Server()" << std::endl;
}

void Server::Start() {
    if (!m_is_started) {
        std::cout << "Server::Start() : start" << std::endl;
        
        m_is_started = true;

        th = std::async(std::launch::async, &Server::start_listen, this);
        return;
    }

    std::cout << "Server::Start() : Already started!" << std::endl;
}

void Server::Stop() {
    if (m_is_started) {

        m_socket->Shutdown(CSimpleSocket::Both);
        m_socket->Close();

        m_is_started = false;

        th.wait();

        std::cout << "Server::Stop() : stop" << std::endl;
        return;
    }

    std::cout << "Server::Stop() : Already stopped!" << std::endl;
}



bool Server::send_msg(CActiveSocketPtr socket_ptr, const uint8_t* src, size_t size) {

    if (!socket_ptr->IsSocketValid()) {
        return false;
    }

    const auto val = socket_ptr->Send(src, size);
    if (val == -1) {
        std::cout << "Server::send_msg() : an error has occurred" << std::endl;
        return false;
    }
    if (val == 0) {
        std::cout << "Server::send_msg() : the connection has been shutdown on the other side" << std::endl;
        return false;
    }
    if (val == (int32_t) size) {
        std::cout << "Server::send_msg() b: " << val << std::endl;
        return true;
    } 
    else {
        std::cout << "Server::send_msg() : " << val <<  " bytes out of " << size << std::endl;
        return false;
    }
}

bool Server::receive_msg(CActiveSocketPtr socket_ptr, std::string& dst, size_t size) {

    if (!socket_ptr->IsSocketValid()) {
        return false;
    }

    const auto val = socket_ptr->Receive(size);
    if (val == -1) {
        std::cout << "Server::receive_msg() : an error has occurred" << std::endl;
        return false;
    }
    if (val == 0) {
        std::cout << "Server::receive_msg() : the connection has been shutdown on the other side" << std::endl;
        return false;
    }
    if (val == (int32)size) {
        dst = std::string(socket_ptr->GetData(), socket_ptr->GetData() + socket_ptr->GetBytesReceived());     
        std::cout << "Server::receive_msg() m: "<< dst << " b: " << val << std::endl;
        return true;
    }
    else {
        std::cout << "Server::receive_msg() : " << val <<  " bytes out of " << size << std::endl;
        return false;
    }
}


void Server::start_listen() {
    std::cout << "Server::start_listen() : start  " << std::this_thread::get_id() << std::endl;
    m_client = std::shared_ptr<CActiveSocket>(m_socket->Accept());
    if (!m_client) {
        std::cout << "Server::start_listen Accept returned nullptr" << std::endl;
        return;
    }

    std::string dst{};
    uint8_t buffer[MAX_BUFFER_SIZE];
    ssize_t block_num = 0, block_size = 0;
    while(m_is_started) {

        if(!receive_msg(m_client, dst, MAX_HASH)) {
            break;
        }

        block_num = m_block_device->get_block_number(dst);
        if (block_num == -1) {
            std::cout << "get_block_number -1" << std::endl;
            break;
        }

        block_size = m_block_device->get_block_size(block_num);
        if (block_size == -1) {
            std::cout << "get_block_size -1" << std::endl;
            break;
        }

        if (m_block_device->get_block_data(block_num, (char*)buffer, block_size) == -1) {
            std::cout << "get_block_data -1" << std::endl;
            break;
        }

        if(!send_msg(m_client, buffer, block_size)) {
            break;
        }
    }

    m_client->Shutdown(CSimpleSocket::Both);
    m_client->Close();

    std::cout << "Server::start_listen() : stop" << std::endl;
}
