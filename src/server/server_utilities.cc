// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/live-memory/element_container_cache.h"
#include "server_utilities.h"
#include "server_endpoints.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <ctime>

namespace pandora {

    namespace server_utilities {

        static std::random_device rd;
        static std::mt19937 seed(rd());
        static std::uniform_int_distribution<int> range(10000000, 99999999);

        void CreateDirectory(const std::filesystem::path path) {
            try {
                std::filesystem::create_directories(path);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not create the " + std::string(path) + " directory.");
                throw std::runtime_error(error);
            }
        }

        DateTime GetDateTime() {
            int two_digits {2};
            int initial_position {0};

            time_t now = time(0);
            tm *ltm = localtime(&now);
            
            DateTime dt;
            dt.year = std::to_string(1900 + ltm->tm_year);
            dt.month = std::to_string(1 + ltm->tm_mon);
            dt.day = std::to_string(ltm->tm_mday);
            dt.hours = std::to_string(5+ltm->tm_hour);
            dt.minutes = std::to_string(30+ltm->tm_min);
            dt.seconds = std::to_string(ltm->tm_sec);

            // Care for two digits for all data
            if(dt.month.size() < two_digits) dt.month.insert(initial_position, "0");
            if(dt.day.size() < two_digits) dt.day.insert(initial_position, "0");
            if(dt.hours.size() < two_digits) dt.hours.insert(initial_position, "0");
            if(dt.minutes.size() < two_digits) dt.minutes.insert(initial_position, "0");
            if(dt.seconds.size() < two_digits) dt.seconds.insert(initial_position, "0");

            return dt;
        }

        void SetEndpoints(httpserver::webserver& pandora_storage_server, std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions& server_options) {

            // Create Elements Container (PUT)
            static pandora::server_endpoints::CreateElementContainerEndpoint create_element_container_endpoint(main_cache, &server_options);
            RegisterEndpoint(pandora_storage_server, create_element_container_endpoint, pandora::server_constants::http_put, 
                                                        pandora::server_constants::create_element_container_endpoint_url);
            // Delete Elements Container (DELETE)
            static pandora::server_endpoints::DeleteElementContainerEndpoint delete_element_container_endpoint;
            RegisterEndpoint(pandora_storage_server, delete_element_container_endpoint, pandora::server_constants::http_delete, 
                                                        pandora::server_constants::delete_element_container_endpoint_url);
            // Set Element (POST)
            static pandora::server_endpoints::SetElementEndpoint set_element_endpoint;
            RegisterEndpoint(pandora_storage_server, set_element_endpoint, pandora::server_constants::http_post, 
                                                        pandora::server_constants::set_element_endpoint_url);
            // Get Element (GET)
            static pandora::server_endpoints::GetElementEndpoint get_element_endpoint;
            RegisterEndpoint(pandora_storage_server, get_element_endpoint, pandora::server_constants::http_get, 
                                                        pandora::server_constants::get_element_endpoint_url);
            // Delete Element (DELETE)
            static pandora::server_endpoints::DeleteElementEndpoint delete_element_endpoint;
            RegisterEndpoint(pandora_storage_server, delete_element_endpoint, pandora::server_constants::http_delete, 
                                                        pandora::server_constants::delete_element_endpoint_url);

        }

        void CreateBaseDirectories() {
            // Create Pandora Storage Server directory
            pandora::server_utilities::CreateDirectory(pandora::server_constants::pandora_directory_path);
            // Create main storage directory
            pandora::server_utilities::CreateDirectory(pandora::server_constants::storage_directory_path);
            // Create logs directory
            pandora::server_utilities::CreateDirectory(pandora::server_constants::logs_directory_path);
            // Create elements storage directory
            pandora::server_utilities::CreateDirectory(pandora::server_constants::element_containers_directory_path);
        }

        std::string GetRandomString_Size8() { return std::to_string(range(seed)); }

        std::string GenerateServerSessionID() {
            // Server session ID (8 first digits correspond to date, 8 latter digits are randomly generated)
            std::string server_session_id {};
            // Date-Time Identifier
            server_session_id.append(GetDateTime().year + 
                                    GetDateTime().month +
                                    GetDateTime().day + "-");
            // Random identifier
            server_session_id.append(GetRandomString_Size8());
            return std::move(server_session_id);
        }

        std::string GenerateTransactionID() {
            std::string transaction_id {};
            transaction_id.append(GetRandomString_Size8() + "-" + GetRandomString_Size8());
            return std::move(transaction_id);
        }

    }

} // namespace pandora