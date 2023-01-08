// *************************************
// Pandora Storage Server
// Server
// 'server_endpoints.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/element_container_operations.h"
#include "../storage/core/element_operations.h"
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

        // Response object wrapper
        std::shared_ptr<httpserver::string_response> response(const std::string& response_string, int http_code) {
            return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(response_string, http_code, "text/plain")); 
        }

        // Base Endpoint Constructor
        BaseEndpoint::BaseEndpoint(std::shared_ptr<pandora::MainData>& main_data) {
            m_main_data = main_data;
        }

        // *********** Create Element Container ***********
        // Constructor
        CreateElementContainerEndpoint::CreateElementContainerEndpoint(std::shared_ptr<pandora::MainData>& main_data)
        : BaseEndpoint(main_data) 
        {}

        // Endpoint | Arguments: element_container_name
        std::shared_ptr<httpserver::http_response> CreateElementContainerEndpoint::render_PUT(const httpserver::http_request& request) {

            auto start = std::chrono::high_resolution_clock::now();

            // Transaction Data creation
            pandora::utilities::TransactionData transaction_data(pandora::utilities::GenerateTransactionID(),
                                                        std::string(request.get_path()), pandora::constants::http_put, start);
            transaction_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);

            try {

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, transaction_data);

                pandora::utilities::ValidateElementContainerName(transaction_data, m_main_data->GetServerOptions());

                pandora::core::CreateElementContainer(m_main_data, transaction_data);

                auto stop = std::chrono::high_resolution_clock::now();
                std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(start, stop);

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, transaction_data, ellapsed_milliseconds);
                m_main_data->GetServerOptions()->LogToFile(transaction_data);
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + ellapsed_milliseconds + "} | " + "Element Container '" + 
                                        transaction_data.arguments[pandora::constants::element_container_name] + "' was created succesfully.");

                return response(transaction_data.log, pandora::constants::http_ok);

            } catch(std::runtime_error error) {

                // Unlock all locks related to this operation
                m_main_data->UnlockExclusiveElementContainerOperations();
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + transaction_data.ellapsed_milliseconds + "} " + std::string(error.what()));

                return response(transaction_data.log, pandora::constants::http_internal_error);
            }

        }
        // ******************** END ***********************

        // *********** Delete Element Container ***********
        // Constructor
        DeleteElementContainerEndpoint::DeleteElementContainerEndpoint(std::shared_ptr<pandora::MainData>& main_data)
        : BaseEndpoint(main_data) 
        {}

        // Endpoint | Arguments: element_container_name
        std::shared_ptr<httpserver::http_response> DeleteElementContainerEndpoint::render_DELETE(const httpserver::http_request& request) {
            
            auto start = std::chrono::high_resolution_clock::now();

            // Request Data creation
            pandora::utilities::TransactionData transaction_data(pandora::utilities::GenerateTransactionID(),
                                                         std::string(request.get_path()), pandora::constants::http_delete, start);
            transaction_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);

            try {

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, transaction_data);

                pandora::utilities::ValidateElementContainerName(transaction_data, m_main_data->GetServerOptions());

                pandora::core::DeleteElementContainer(m_main_data, transaction_data);

                auto stop = std::chrono::high_resolution_clock::now();
                std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(start, stop);

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, transaction_data, ellapsed_milliseconds);
                m_main_data->GetServerOptions()->LogToFile(transaction_data);
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + ellapsed_milliseconds + "} | " + "Element Container '" + 
                                        transaction_data.arguments[pandora::constants::element_container_name] + "' was deleted succesfully.");

                return response(transaction_data.log, pandora::constants::http_ok);

            } catch(std::runtime_error error) {

                // Unlock all mutex locks related to this operation
                m_main_data->UnlockExclusiveElementContainerOperations();
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + transaction_data.ellapsed_milliseconds + "} " + std::string(error.what()));

                return response(transaction_data.log, pandora::constants::http_internal_error);
            }

        }
        // ******************** END ***********************

        // *********** Set Element ***********
        // Constructor
        SetElementEndpoint::SetElementEndpoint(std::shared_ptr<pandora::MainData>& main_data)
        : BaseEndpoint(main_data) 
        {}

        // Endpoint | Arguments: element_container_name/element_id/element_value
        std::shared_ptr<httpserver::http_response> SetElementEndpoint::render_POST(const httpserver::http_request& request) {
            
            auto start = std::chrono::high_resolution_clock::now();

            // Request Data creation
            pandora::utilities::TransactionData transaction_data(pandora::utilities::GenerateTransactionID(),
                                                         std::string(request.get_path()), pandora::constants::http_post, start);
            transaction_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);
            transaction_data.arguments[pandora::constants::element_id] = request.get_arg(pandora::constants::element_id);
            transaction_data.arguments[pandora::constants::element_value] = request.get_arg(pandora::constants::element_value);

            try {

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, transaction_data);

                pandora::utilities::ValidateElementContainerName(transaction_data, m_main_data->GetServerOptions());
                pandora::utilities::ValidateElementID(transaction_data, m_main_data->GetServerOptions());
                pandora::utilities::ValidateElementValue(transaction_data, m_main_data->GetServerOptions());

                pandora::core::SetElement(m_main_data, transaction_data);

                auto stop = std::chrono::high_resolution_clock::now();
                std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(start, stop);

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, transaction_data, ellapsed_milliseconds);
                m_main_data->GetServerOptions()->LogToFile(transaction_data);
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + ellapsed_milliseconds + "} | " + "Element '" +
                                         transaction_data.arguments[pandora::constants::element_id] + "' was set succesfully.");

                return response(transaction_data.log, pandora::constants::http_ok);

            } catch(std::runtime_error error) {

                // Unlock all mutex locks related to this operation
                if(m_main_data->ElementContainerExists(transaction_data.arguments[pandora::constants::element_container_name])) 
                    m_main_data->GetElementContainer(transaction_data.arguments[pandora::constants::element_container_name]).UnlockExclusiveElementOperations();
                m_main_data->UnlockSharedElementContainerOperations();
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + transaction_data.ellapsed_milliseconds + "} " + std::string(error.what()));
                
                return response(transaction_data.log, pandora::constants::http_internal_error);
            }

        }
        // ******************** END ***********************

        // *********** Get Element ***********
        // Constructor
        GetElementEndpoint::GetElementEndpoint(std::shared_ptr<pandora::MainData>& main_data)
        : BaseEndpoint(main_data) 
        {}

        // Endpoint | Arguments: element_container_name/element_id
        std::shared_ptr<httpserver::http_response> GetElementEndpoint::render_GET(const httpserver::http_request& request) {
            
            auto start = std::chrono::high_resolution_clock::now();

            // Request Data creation
            pandora::utilities::TransactionData transaction_data(pandora::utilities::GenerateTransactionID(),
                                                         std::string(request.get_path()), pandora::constants::http_get, start);
            transaction_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);
            transaction_data.arguments[pandora::constants::element_id] = request.get_arg(pandora::constants::element_id);

            try {

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, transaction_data);

                pandora::utilities::ValidateElementContainerName(transaction_data, m_main_data->GetServerOptions());
                pandora::utilities::ValidateElementID(transaction_data, m_main_data->GetServerOptions());

                std::string element_value = pandora::core::GetElement(m_main_data, transaction_data);

                auto stop = std::chrono::high_resolution_clock::now();
                std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(start, stop);

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, transaction_data, ellapsed_milliseconds);
                m_main_data->GetServerOptions()->LogToFile(transaction_data);
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + ellapsed_milliseconds + "} | " + element_value);
                
                return response(transaction_data.log, pandora::constants::http_ok);

            } catch(std::runtime_error error) {

                // Unlock all mutex locks related to this operation
                if(m_main_data->ElementContainerExists(transaction_data.arguments[pandora::constants::element_container_name])) 
                    m_main_data->GetElementContainer(transaction_data.arguments[pandora::constants::element_container_name]).UnlockSharedElementOperations();
                m_main_data->UnlockSharedElementContainerOperations();
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + transaction_data.ellapsed_milliseconds + "} " + std::string(error.what()));
                
                return response(transaction_data.log, pandora::constants::http_internal_error);
            }

        }
        // ******************** END ***********************

        // *********** Delete Element ***********
        // Constructor
        DeleteElementEndpoint::DeleteElementEndpoint(std::shared_ptr<pandora::MainData>& main_data)
        : BaseEndpoint(main_data) 
        {}

        // Endpoint | Arguments: element_container_name/element_id
        std::shared_ptr<httpserver::http_response> DeleteElementEndpoint::render_DELETE(const httpserver::http_request& request) {
            
            auto start = std::chrono::high_resolution_clock::now();

            // Request Data creation
            pandora::utilities::TransactionData transaction_data(pandora::utilities::GenerateTransactionID(),
                                                         std::string(request.get_path()), pandora::constants::http_delete, start);
            transaction_data.arguments[pandora::constants::element_container_name] = request.get_arg(pandora::constants::element_container_name);
            transaction_data.arguments[pandora::constants::element_id] = request.get_arg(pandora::constants::element_id);

            try {

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionStartedCode, transaction_data);

                pandora::utilities::ValidateElementContainerName(transaction_data, m_main_data->GetServerOptions());
                pandora::utilities::ValidateElementID(transaction_data, m_main_data->GetServerOptions());

                pandora::core::DeleteElement(m_main_data, transaction_data);

                auto stop = std::chrono::high_resolution_clock::now();
                std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(start, stop);

                m_main_data->GetServerOptions()->LogTransactionStartedFinished(pandora::constants::TransactionFinishedCode, transaction_data, ellapsed_milliseconds);
                m_main_data->GetServerOptions()->LogToFile(transaction_data);
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + ellapsed_milliseconds + "} | " + "Element '" +
                                         transaction_data.arguments[pandora::constants::element_id] + "' was deleted succesfully.");
                
                return response(transaction_data.log, pandora::constants::http_ok);

            } catch(std::runtime_error error) {

                // Unlock all mutex locks related to this operation
                if(m_main_data->ElementContainerExists(transaction_data.arguments[pandora::constants::element_container_name])) 
                    m_main_data->GetElementContainer(transaction_data.arguments[pandora::constants::element_container_name]).UnlockExclusiveElementOperations();
                m_main_data->UnlockSharedElementContainerOperations();
                
                transaction_data.log.append("[" + transaction_data.transaction_id + "] " + "{" + transaction_data.ellapsed_milliseconds + "} " + std::string(error.what()));
                
                return response(transaction_data.log, pandora::constants::http_internal_error);
            }

        }
        // ******************** END ***********************

    }

} // namespace pandora