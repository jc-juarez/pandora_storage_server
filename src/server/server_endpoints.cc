// *************************************
// Pandora Storage Server
// Server
// 'server_endpoints.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "server_endpoints.h"
#include "server_utilities.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <string>

namespace pandora {

    namespace server_endpoints {
        
        // Resource not found
        std::shared_ptr<httpserver::http_response> resource_not_found(const httpserver::http_request& request) {
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Pandora could not find the requested resource.", 404, "text/plain"));
        }

        // HTTP Method not allowed
        std::shared_ptr<httpserver::http_response> method_not_allowed(const httpserver::http_request& request) {
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Pandora could not handle the requested HTTP method.", 405, "text/plain"));
        }

        // Create Elements Container
        std::shared_ptr<httpserver::http_response> CreateElementsContainerEndpoint::render_PUT(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::server_utilities::GetRandomString_Size8() + "-" + pandora::server_utilities::GetRandomString_Size8());
            pandora::server_utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"PUT from"};
            response.append(std::to_string(pandora::server_options::port_number));

            pandora::server_utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }

        // Get Elements Container
        std::shared_ptr<httpserver::http_response> GetElementsContainerEndpoint::render_GET(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::server_utilities::GetRandomString_Size8() + "-" + pandora::server_utilities::GetRandomString_Size8());
            pandora::server_utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"GET from"};
            response.append(std::to_string(pandora::server_options::port_number));

            pandora::server_utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }

        // Delete Elements Container
        std::shared_ptr<httpserver::http_response> DeleteElementsContainerEndpoint::render_DELETE(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::server_utilities::GetRandomString_Size8() + "-" + pandora::server_utilities::GetRandomString_Size8());
            pandora::server_utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"DELETE from"};
            response.append(std::to_string(pandora::server_options::port_number));

            pandora::server_utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }

    }

} // namespace pandora