// *************************************
// Pandora Storage Server
// Server
// 'main.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/orion_engine.h"
#include "../storage/storage_operations.h"
#include "server_endpoints.h"
#include "server_utilities.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unistd.h>

// External server options definition
namespace pandora {

    namespace server_options {

        int port_number {};
        bool debug_enabled {};
        bool logs_enabled {};
        std::string server_session_id {};
        std::string logs_file_path {};

    }

} // namespace pandora

int main(int argc, char** argv) {

    // Check for process running with privileges
    if(getuid()) throw std::runtime_error("Pandora could not start without root user privileges. Run the process as root.");

    // Initial server configiration
    pandora::server_options::port_number = pandora::default_options::port_number;
    pandora::server_options::debug_enabled = pandora::default_options::debug_enabled;
    pandora::server_options::logs_enabled = pandora::default_options::logs_enabled;

    // Server session ID (8 first digits correspond to date, 8 latter digits are randomly generated)
    // Date-Time Identifier
    std::string datetime_id {};
    datetime_id.append(pandora::server_utilities::GetDateTime().year + 
                       pandora::server_utilities::GetDateTime().month +
                       pandora::server_utilities::GetDateTime().day + "-");
    pandora::server_options::server_session_id.append(datetime_id);
    // Random identifier
    std::string random_id {pandora::server_utilities::GetRandomString_Size8()};  
    pandora::server_options::server_session_id.append(random_id);

    // Command-line arguments for server startup
    // p={Port Number: #} d={Debug enabled: off} l={Logs enabled: off}
    std::vector<std::string> args(argv, argv + argc);
    
    // Options updates based on commands
    if(!args.empty()) {

        int option_command_index {0};
        int option_value_index {2};
        int minimum_command_size {3};
        for(const std::string& command : args) {

            // Care for malformed commands
            if(command.size() < minimum_command_size) continue;

            char command_option {command[option_command_index]};
            std::string option_value {command.substr(option_value_index)};

            switch (command_option) {
                case pandora::server_constants::port_number_option :
                    pandora::server_options::port_number = std::stoi(option_value);
                    break;
                case pandora::server_constants::debug_enabled_option :
                    if(option_value == pandora::server_constants::off_option) pandora::server_options::debug_enabled = false;
                    break;
                case pandora::server_constants::logs_enabled_option :
                    if(option_value == pandora::server_constants::off_option) pandora::server_options::logs_enabled = false;
                    break;
            }
        }
    }

    // Server creation
    httpserver::webserver pandora_storage_server = httpserver::create_webserver(pandora::server_options::port_number)
                               .not_found_resource(pandora::server_endpoints::resource_not_found)
                               .method_not_allowed_resource(pandora::server_endpoints::method_not_allowed);

    // Server Endpoints referencing
    // Create Elements Container (PUT)
    pandora::server_endpoints::CreateElementsContainerEndpoint create_elements_container_endpoint;
    pandora::server_utilities::RegisterEndpoint(pandora_storage_server, create_elements_container_endpoint, std::string(pandora::server_constants::http_put), 
                                                std::string(pandora::server_constants::create_elements_container_endpoint_url));
    // Delete Elements Container (DELETE)
    pandora::server_endpoints::DeleteElementsContainerEndpoint delete_elements_container_endpoint;
    pandora::server_utilities::RegisterEndpoint(pandora_storage_server, delete_elements_container_endpoint, std::string(pandora::server_constants::http_delete), 
                                                std::string(pandora::server_constants::delete_elements_container_endpoint_url));
    // Set Element (POST)
    pandora::server_endpoints::SetElementEndpoint set_element_endpoint;
    pandora::server_utilities::RegisterEndpoint(pandora_storage_server, set_element_endpoint, std::string(pandora::server_constants::http_post), 
                                                std::string(pandora::server_constants::set_element_endpoint_url));
    // Get Element (GET)
    pandora::server_endpoints::GetElementEndpoint get_element_endpoint;
    pandora::server_utilities::RegisterEndpoint(pandora_storage_server, get_element_endpoint, std::string(pandora::server_constants::http_get), 
                                                std::string(pandora::server_constants::get_element_endpoint_url));
    // Delete Element (DELETE)
    pandora::server_endpoints::DeleteElementEndpoint delete_element_endpoint;
    pandora::server_utilities::RegisterEndpoint(pandora_storage_server, delete_element_endpoint, std::string(pandora::server_constants::http_delete), 
                                                std::string(pandora::server_constants::delete_element_endpoint_url));
    
    // Start Pandora Storage Server in non-blocking mode
    pandora_storage_server.start(false);

    // Create Pandora's directories (requires sudo)
    // Create Pandora Storage Server directory
    pandora::server_utilities::CreateDirectory(std::string(pandora::server_constants::pandora_directory_path));
    // Create main storage directory
    pandora::server_utilities::CreateDirectory(std::string(pandora::server_constants::storage_directory_path));
    // Create logs directory
    pandora::server_utilities::CreateDirectory(std::string(pandora::server_constants::logs_directory_path));
    // Create elements storage directory
    pandora::server_utilities::CreateDirectory(std::string(pandora::server_constants::elements_directory_path));
    // Create files storage directory
    pandora::server_utilities::CreateDirectory(std::string(pandora::server_constants::files_directory_path));

    // Initial Live Memory Filling
    pandora::core::InitialLiveMemoryFilling();

    // Server startup messages
    std::cout << "\n<<< Pandora Storage Server >>>\n\n";
    pandora::server_utilities::ConsoleLog("Debug mode: " + (pandora::server_options::debug_enabled ? std::string("Enabled") : std::string("Disabled")));
    pandora::server_utilities::ConsoleLog("Logs: " + (pandora::server_options::logs_enabled ? std::string("Enabled") : std::string("Disabled")));
    pandora::server_utilities::ConsoleLog("Server Session ID: " + pandora::server_options::server_session_id);
    
    // Logs file
    if(pandora::server_options::logs_enabled) {
        std::string logs_file_path {};
        logs_file_path.append(std::string(pandora::server_constants::logs_directory_path) + "/pandoralog-" + pandora::server_options::server_session_id + ".txt");
        pandora::server_options::logs_file_path = logs_file_path;
        pandora::storage_operations::AddFileContent(pandora::server_options::logs_file_path, "", false);
        pandora::server_utilities::ConsoleLog("Logs will be recorded at file: " + pandora::server_options::logs_file_path);
    }

    if(pandora::server_options::debug_enabled) std::cout << "\n";
    std::cout << "* Running on port: " << pandora::server_options::port_number << "\n\n";

    // Run forever
    for(;;) {}
    
    return 0;
}