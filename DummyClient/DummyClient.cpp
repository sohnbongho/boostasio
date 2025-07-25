#include <boost/asio/io_context.hpp>
#include "DummyTcpClient.h"

int main()
{
    try
    {
        ::Sleep(1000);

        boost::asio::io_context io_context;

        DummyTcpClient client(io_context, "127.0.0.1", 12345); // 서버 주소/포트에 맞게
        std::thread ioThread([&]() { io_context.run(); });

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto data = std::make_shared<std::string>(input + "\n");
            client.Send(data);
        }

        ioThread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
