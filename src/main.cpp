/*
 * Part of HTTPP.
 *
 * Distributed under the 2-clause BSD licence (See LICENCE.TXT file at the
 * project root).
 *
 * Copyright (c) 2014 Thomas Sanchez.  All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <chrono>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

using HTTPP::HttpServer;
using HTTPP::HTTP::Request;
using HTTPP::HTTP::Connection;
using HTTPP::HTTP::HttpCode;

void handler(Connection *connection) {
    read_whole_request(connection, [](std::unique_ptr <
                                      HTTPP::HTTP::helper::ReadWholeRequest> hndl,
                                      const boost::system::error_code &ec) {
        const auto &body = hndl->body;
        const auto &connection = hndl->connection;
        const auto &request = connection->request();

        if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
        }

        std::ostringstream out;
        out << request;

        std::string responseBody;
        if (body.empty()) {
            responseBody = "request received: " + out.str();
        } else {
            unsigned long bodySize = body.size();
            std::string bodyContent(body.begin(), body.end());
            responseBody = "request received entirely: " + out.str() +
                                       ", body size: " + std::to_string(bodySize) +
                                       "\n\nBody:\n" + bodyContent;
        }
        connection->response().setCode(HttpCode::Ok).setBody(boost::string_ref(responseBody));

        HTTPP::HTTP::setShouldConnectionBeClosed(request, connection->response());
        connection->sendResponse(); // connection pointer may become invalid

        auto end = Request::Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            end - request.received);
        std::cout << "Request handled in: " << elapsed.count() << "us"
                  << std::endl;
    });
}

int main(int, char **) {
    HttpServer server;
    server.start();
    server.setSink(&handler);
    server.bind("0.0.0.0", "8080");
    server.bind("localhost", "8081");
    server.bind("localhost", "8082");

    // Loop until EOF
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.find(27) != std::string::npos)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Stopping server." << std::endl;

    return 0;
}
