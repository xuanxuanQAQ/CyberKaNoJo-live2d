#include <TcpClient.hpp>

TcpClient& TcpClient::GetInstance() {
    static TcpClient instance;
    return instance;
}

TcpClient::TcpClient() : io_context_(), socket_(io_context_) {
    tcp::resolver resolver(io_context_);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket_, endpoints);
    start_receive();
}

void TcpClient::start_receive() {
    socket_.async_read_some(
        boost::asio::buffer(receive_buffer_),
        [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                for (std::size_t i = 0; i < bytes_transferred; ++i) {
                    received_int = (received_int << 8) | receive_buffer_[i];
                }
                data_available_ = true;
                start_receive();
            }
            else {
                throw boost::system::system_error(error);
            }
        });
}

void TcpClient::poll() {
    io_context_.poll();
}

int32_t TcpClient::get_received_data() {
    if (data_available_) {
        data_available_ = false;
        return received_int;
    }
    return 0;
}

const char TcpClient::host[10] = "127.0.0.1";
const char TcpClient::port[6] = "12345";