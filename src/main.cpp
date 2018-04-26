/*
 * Modified sample from HTTPP.
 *
 * Distributed under the 2-clause BSD licence (See LICENCE.TXT file at the HTTPP
 * project root).
 */

#include <iostream>
#include <string>
#include <chrono>

#include <httpp/HttpServer.hpp>
#include <httpp/http/RestDispatcher.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

using HTTPP::HttpServer;
using HTTPP::HTTP::Request;
using HTTPP::HTTP::Connection;
using HTTPP::HTTP::RestDispatcher;
using HTTPP::HTTP::helper::ReadWholeRequest;
using HTTPP::HTTP::HttpCode;
using HTTPP::HTTP::Method;
using HTTPP::HTTP::Route;

void handler_without_body(Connection* connection) {
    connection->response().setCode(HttpCode::Ok).setBody(boost::string_ref("Hello GET!"));
    HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(), connection->response());
    connection->sendResponse(); // connection pointer may become invalid
}

void handler_with_body(ReadWholeRequest::Handle handle) {
    auto connection = handle->connection;
    // do something with handle->body
    connection->response().setCode(HttpCode::Ok).setBody(boost::string_ref("Hello POST!"));
    HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(), connection->response());
    connection->sendResponse(); // connection pointer may become invalid
}

int main(int, char **) {
    commonpp::core::init_logging();
    commonpp::core::set_logging_level(commonpp::debug);
    HttpServer server(1);
    server.start();

    RestDispatcher dispatcher(server);
    dispatcher.add<Method::GET>("/", (Route::WithoutBodyHandler) &handler_without_body);
    dispatcher.add<Method::POST>("/", (Route::WithBodyHandler) &handler_with_body);

    dispatcher.add<Method::PUT, Method::DELETE_, Method::HEAD, Method::CONNECT>(
        "/", (Route::WithoutBodyHandler) [](Connection *connection) {
            connection->response().setCode(HttpCode::Forbidden);
            HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(),
                                                     connection->response());
            connection->sendResponse(); // connection pointer may become invalid
        });

    server.bind("0.0.0.0", "8080");

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
