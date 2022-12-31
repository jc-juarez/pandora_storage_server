// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/main_data.h"
#include "../storage/storage.h"
#include "server_utilities.h"
#include "server_endpoints.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <iostream>
#include <random>
#include <string>
#include <ctime>

namespace pandora {

    namespace utilities {

        static std::random_device rd;
        static std::mt19937 seed(rd());
        static std::uniform_int_distribution<int> range_8(10000000, 99999999);

        DateTime GetDateTime() {
            int two_digits {2};
            int initial_position {0};

            time_t now = time(0);
            tm *ltm = localtime(&now);
            
            DateTime dt;
            dt.year = std::to_string(1900 + ltm->tm_year);
            dt.month = std::to_string(1 + ltm->tm_mon);
            dt.day = std::to_string(ltm->tm_mday);
            dt.hours = std::to_string(ltm->tm_hour);
            dt.minutes = std::to_string(ltm->tm_min);
            dt.seconds = std::to_string(ltm->tm_sec);

            // Care for two digits for all data
            if(dt.month.size() < two_digits) dt.month.insert(initial_position, "0");
            if(dt.day.size() < two_digits) dt.day.insert(initial_position, "0");
            if(dt.hours.size() < two_digits) dt.hours.insert(initial_position, "0");
            if(dt.minutes.size() < two_digits) dt.minutes.insert(initial_position, "0");
            if(dt.seconds.size() < two_digits) dt.seconds.insert(initial_position, "0");

            return dt;
        }

        void SetEndpoints(httpserver::webserver& pandora_storage_server, std::shared_ptr<pandora::MainData>& main_data) {

            // Create Elements Container (PUT)
            static pandora::endpoints::CreateElementContainerEndpoint create_element_container_endpoint(main_data);
            RegisterEndpoint(pandora_storage_server, create_element_container_endpoint, pandora::constants::http_put, 
                                                        pandora::constants::create_element_container_endpoint_url);
            // Delete Elements Container (DELETE)
            static pandora::endpoints::DeleteElementContainerEndpoint delete_element_container_endpoint(main_data);
            RegisterEndpoint(pandora_storage_server, delete_element_container_endpoint, pandora::constants::http_delete, 
                                                        pandora::constants::delete_element_container_endpoint_url);
            // Set Element (POST)
            static pandora::endpoints::SetElementEndpoint set_element_endpoint(main_data);
            RegisterEndpoint(pandora_storage_server, set_element_endpoint, pandora::constants::http_post, 
                                                        pandora::constants::set_element_endpoint_url);
            // Get Element (GET)
            static pandora::endpoints::GetElementEndpoint get_element_endpoint(main_data);
            RegisterEndpoint(pandora_storage_server, get_element_endpoint, pandora::constants::http_get, 
                                                        pandora::constants::get_element_endpoint_url);
            // Delete Element (DELETE)
            static pandora::endpoints::DeleteElementEndpoint delete_element_endpoint(main_data);
            RegisterEndpoint(pandora_storage_server, delete_element_endpoint, pandora::constants::http_delete, 
                                                        pandora::constants::delete_element_endpoint_url);

        }

        void CreateBaseDirectories() {
            // Create Pandora Storage Server directory
            pandora::storage::CreateDirectory(pandora::constants::pandora_directory_path);
            // Create logs directory
            pandora::storage::CreateDirectory(pandora::constants::logs_directory_path);
            // Create element containers storage directory
            pandora::storage::CreateDirectory(pandora::constants::element_containers_directory_path);
        }

        void ValidateElementContainerName(RequestData& request_data, pandora::ServerOptions* server_options) {
            // Element Container name should not be empty
            if(request_data.arguments[pandora::constants::element_container_name].empty()) {
                request_data.log.append("Element Container name cannot be empty.");
                server_options->LogError(pandora::constants::ParameterEmptyErrorCode, request_data);
            }
            // Element Container name should not exceed ElementContainerNameMaxSize
            if(request_data.arguments[pandora::constants::element_container_name].size() > pandora::constants::ElementContainerNameMaxSize) {
                request_data.log.append("Element Container name exceeds max size of " + std::to_string(pandora::constants::ElementContainerNameMaxSize) + " characters.");
                server_options->LogError(pandora::constants::ParameterOversizeErrorCode, request_data);
            }
        }

        void ValidateElementID(RequestData& request_data, pandora::ServerOptions* server_options) {
            // Element ID should not be empty
            if(request_data.arguments[pandora::constants::element_id].empty()) {
                request_data.log.append("Element ID cannot be empty.");
                server_options->LogError(pandora::constants::ParameterEmptyErrorCode, request_data);
            }
            // Element ID should not exceed ElementIDMaxSize
            if(request_data.arguments[pandora::constants::element_id].size() > pandora::constants::ElementIDMaxSize) {
                request_data.log.append("Element ID exceeds max size of " + std::to_string(pandora::constants::ElementIDMaxSize) + " characters.");
                server_options->LogError(pandora::constants::ParameterOversizeErrorCode, request_data);
            }
        }

        void ValidateElementValue(RequestData& request_data, pandora::ServerOptions* server_options) {
            // Element Value should not be empty
            if(request_data.arguments[pandora::constants::element_value].empty()) {
                request_data.log.append("Element Value cannot be empty.");
                server_options->LogError(pandora::constants::ParameterEmptyErrorCode, request_data);
            }
            // Element Value should not exceed ElementValueMaxSize
            if(request_data.arguments[pandora::constants::element_value].size() > pandora::constants::ElementValueMaxSize) {
                request_data.log.append("Element Value exceeds max size of " + std::to_string(pandora::constants::ElementValueMaxSize) + " characters.");
                server_options->LogError(pandora::constants::ParameterOversizeErrorCode, request_data);
            }
        }

        std::string GetRandomString_Size8() { return std::to_string(range_8(seed)); }

        std::string GenerateServerSessionID() {
            // Server session ID (8 first digits correspond to date, 8 latter digits are randomly generated)
            std::string server_session_id {};
            // Date-Time Identifier
            DateTime date_time = GetDateTime();
            server_session_id.append(date_time.year + 
                                     date_time.month +
                                     date_time.day + "-");
            // Random identifier
            server_session_id.append(GetRandomString_Size8());
            return server_session_id;
        }

        std::string GenerateTransactionID() {
            std::string transaction_id {};
            transaction_id.append(GetRandomString_Size8() + "-" + GetRandomString_Size8());
            return transaction_id;
        }

        std::string GetDateTimeString() {
            // DateTime as string
            std::string date_time_string {};
            DateTime date_time = GetDateTime();
            date_time_string.append(date_time.month + "/" +
                                    date_time.day + "/" + 
                                    date_time.year + " " +
                                    date_time.hours + ":" +
                                    date_time.minutes + ":" +
                                    date_time.seconds);
            return date_time_string;
        }

    }

} // namespace pandora