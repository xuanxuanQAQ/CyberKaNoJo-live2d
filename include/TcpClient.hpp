#pragma once
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
	
class TcpClient {
public: 
    static TcpClient& GetInstance();

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void poll();
    int32_t get_received_data();

    bool is_speaking() const;

    void set_speaking_status(bool value);

private:
    TcpClient();
    void start_receive();

    boost::asio::io_context io_context_;
    tcp::socket socket_;
    static const char host[10];
    static const char port[6];
    boost::array<uint8_t, 4> receive_buffer_;
    int32_t received_int;
    bool data_available_;
    bool speaking_status = false;
};
