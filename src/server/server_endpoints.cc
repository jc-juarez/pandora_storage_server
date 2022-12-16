// *************************************
// Pandora Storage Server
// Server
// 'server_endpoints.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/element_container.h"
#include "server_endpoints.h"
#include "server_utilities.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <sstream>
#include <string>

namespace pandora {

    namespace endpoints {
        
        // Resource not found
        std::shared_ptr<httpserver::http_response> resource_not_found(const httpserver::http_request& request) {
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Pandora could not find the requested resource.", 404, "text/plain"));
        }

        // HTTP Method not allowed
        std::shared_ptr<httpserver::http_response> method_not_allowed(const httpserver::http_request& request) {
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response("Pandora could not handle the requested HTTP method.", 405, "text/plain"));
        }

        // Base Endpoint Constructor
        BaseEndpoint::BaseEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options) {
            m_main_cache = main_cache;
            m_server_options = server_options;
        }

        // *********** Create Element Container ***********
        // Constructor
        CreateElementContainerEndpoint::CreateElementContainerEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options)
        : BaseEndpoint(main_cache, server_options) 
        {}

        // Endpoint | Arguments: element_container_name
        std::shared_ptr<httpserver::http_response> CreateElementContainerEndpoint::render_PUT(const httpserver::http_request& request) {

            try {

                pandora::utilities::RequestData request_data(pandora::utilities::GenerateTransactionID(),
                                                                std::string(request.get_path()), pandora::constants::http_put);
                request_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);
                m_server_options->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, request_data);

                pandora::core::CreateElementContainer(m_main_cache, m_server_options, request_data);

                m_server_options->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, request_data);
                m_server_options->LogToFile(request_data);
                
                std::string response {};
                response.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was created succesfully.");
                return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(response, 200, "text/plain"));

            } catch(std::runtime_error error) {
                std::string error_response {std::string(error.what())};
                return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(error_response, 500, "text/plain"));
            }

        }
        // ******************** END ***********************

        // *********** Delete Element Container ***********
        // Constructor
        DeleteElementContainerEndpoint::DeleteElementContainerEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options)
        : BaseEndpoint(main_cache, server_options) 
        {}

        // Endpoint
        std::shared_ptr<httpserver::http_response> DeleteElementContainerEndpoint::render_DELETE(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::utilities::GetRandomString_Size8() + "-" + pandora::utilities::GetRandomString_Size8());
            //pandora::utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"DELETE from"};
            response.append("Delete Elements Container");

            //pandora::utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }
        // ******************** END ***********************

        // *********** Set Element ***********
        // Constructor
        SetElementEndpoint::SetElementEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options)
        : BaseEndpoint(main_cache, server_options) 
        {}

        // Endpoint
        std::shared_ptr<httpserver::http_response> SetElementEndpoint::render_POST(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::utilities::GetRandomString_Size8() + "-" + pandora::utilities::GetRandomString_Size8());
            //pandora::utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"DELETE from"};
            response.append("Set Element");

            //pandora::utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }
        // ******************** END ***********************

        // *********** Get Element ***********
        // Constructor
        GetElementEndpoint::GetElementEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options)
        : BaseEndpoint(main_cache, server_options) 
        {}

        // Endpoint
        std::shared_ptr<httpserver::http_response> GetElementEndpoint::render_GET(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::utilities::GetRandomString_Size8() + "-" + pandora::utilities::GetRandomString_Size8());
            //pandora::utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"DELETE from"};
            response.append("Get Element");

            //pandora::utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }
        // ******************** END ***********************

        // *********** Delete Element ***********
        // Constructor
        DeleteElementEndpoint::DeleteElementEndpoint(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options)
        : BaseEndpoint(main_cache, server_options) 
        {}

        // Endpoint
        std::shared_ptr<httpserver::http_response> DeleteElementEndpoint::render_DELETE(const httpserver::http_request& request) {
            
            std::string transaction_id {};
            transaction_id.append(pandora::utilities::GetRandomString_Size8() + "-" + pandora::utilities::GetRandomString_Size8());
            //pandora::utilities::ConsoleLog(std::string("Transaction Initiated (0) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            
            std::string response {"DELETE from"};
            response.append("Delete Element");

            //pandora::utilities::ConsoleLog(std::string("Transaction Finished (1) -> [") + transaction_id + std::string("] GET ") + std::string(request.get_path()));
            return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
        }
        // ******************** END ***********************

    }

} // namespace pandora