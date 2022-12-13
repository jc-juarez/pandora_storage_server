// *************************************
// Pandora Storage Server
// Server
// 'main.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/containers_cache.h"
#include "../storage/core/orion_engine.h"
#include "server_endpoints.h"
#include "server_utilities.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

int main(int argc, char** argv) {

    // Check for process running with privileges
    if(getuid()) throw std::runtime_error("Pandora could not start without root user privileges. Run the process as root.");

    // Initial server configuration
    pandora::ServerOptions server_options(pandora::default_options::port_number,
                                          pandora::default_options::debug_enabled,
                                          pandora::default_options::logs_enabled,
                                          std::string(),
                                          std::string());

    // Server session ID (8 first digits correspond to date, 8 latter digits are randomly generated)
    std::string server_session_id {};
    // Date-Time Identifier
    server_session_id.append(pandora::server_utilities::GetDateTime().year + 
                                            pandora::server_utilities::GetDateTime().month +
                                            pandora::server_utilities::GetDateTime().day + "-");
    // Random identifier
    server_session_id.append(pandora::server_utilities::GetRandomString_Size8());
    server_options.SetServerSessionID(server_session_id);

    // Command-line arguments for server startup override
    // p={Port Number: #} d={Debug enabled: off} l={Logs enabled: off}
    std::vector<std::string> args(argv, argv + argc);
    server_options.OverrideOptions(args);

    // Server creation
    httpserver::webserver pandora_storage_server = httpserver::create_webserver(server_options.GetPortNumber())
                               .not_found_resource(pandora::server_endpoints::resource_not_found)
                               .method_not_allowed_resource(pandora::server_endpoints::method_not_allowed);

    // Storage Core: Containers Cache creation
    pandora::ContainersCache main_cache;

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

    // Create Pandora's directories
    pandora::server_utilities::CreateBaseDirectories();

    // Initial Live Memory Filling
    main_cache.InitialLiveMemoryFilling();

    // Server startup messages
    std::cout << "\n<<< Pandora Storage Server >>>\n\n";
    server_options.ConsoleLog("Debug mode: " + (server_options.GetDebugEnabled() ? std::string("Enabled") : std::string("Disabled")));
    server_options.ConsoleLog("Logs: " + (server_options.GetLogsEnabled() ? std::string("Enabled") : std::string("Disabled")));
    server_options.ConsoleLog("Server Session ID: " + server_options.GetServerSessionID());
    
    // Logs file
    if(server_options.GetLogsEnabled()) server_options.CreateLogsFile();

    if(server_options.GetDebugEnabled()) std::cout << "\n";
    std::cout << "* Running on port: " << server_options.GetPortNumber() << "\n\n";

    // Run forever
    for(;;) {}
    
    return 0;
}