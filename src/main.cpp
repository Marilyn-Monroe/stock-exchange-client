#include <iostream>

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include "common.hpp"

std::string ReadMessage(boost::asio::ip::tcp::socket &socket) {
    boost::asio::streambuf b;
    boost::asio::read_until(socket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

std::string ProcessRegistration(boost::asio::ip::tcp::socket &socket) {
    std::string name;
    std::cout << "Hello! Enter your name: ";
    std::cin >> name;

    nlohmann::json req;
    req["ReqType"] = Requests::Registration;
    req["Username"] = name;

    std::string request = req.dump();
    boost::asio::write(socket, boost::asio::buffer(request, request.size()));

    return ReadMessage(socket);
}

std::string ProcessViewBalance(boost::asio::ip::tcp::socket &socket) {
    nlohmann::json req;
    req["ReqType"] = Requests::ViewBalance;

    std::string request = req.dump();
    boost::asio::write(socket, boost::asio::buffer(request, request.size()));

    return ReadMessage(socket);
}

std::string ProcessAddOrder(boost::asio::ip::tcp::socket &socket) {
    nlohmann::json req;
    req["ReqType"] = Requests::AddOrder;
    req["SymbolId"] = 0;
    while (true) {
        std::cout << "Enter side(Buy or Sell):\n";
        std::string side;
        std::cin >> side;
        if (side == "Buy" || side == "Sell") {
            req["Type"] = side;
            break;
        }
    }
    std::cout << "Enter price:\n";
    boost::uint64_t price;
    std::cin >> price;
    req["Price"] = price;

    std::cout << "Enter quantity:\n";
    boost::uint64_t quantity;
    std::cin >> quantity;
    req["Quantity"] = quantity;

    std::string request = req.dump();
    boost::asio::write(socket, boost::asio::buffer(request, request.size()));

    return ReadMessage(socket);
}

int main() {
    try {
        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "127.0.0.1", std::to_string(PORT));
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        boost::asio::ip::tcp::socket s(io_service);
        s.connect(*iterator);

        std::cout << ProcessRegistration(s);

        while (true) {
            std::cout << "Menu:\n"
                         "1) View Balance\n"
                         "2) Add Order\n"
                         "3) Exit\n"
                      << std::endl;

            boost::uint16_t menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num) {
                case 1: {
                    std::cout << ProcessViewBalance(s);
                    break;
                }
                case 2: {
                    std::cout << ProcessAddOrder(s);
                    break;
                }
                case 3: {
                    exit(0);
                }
                default: {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}