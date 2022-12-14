// *************************************
// Pandora Storage Server
// Server
// 'main.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/live-memory/element_container_cache.h"
#include "server_endpoints.h"
#include "server_utilities.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>

int main(int argc, char** argv) {

    // Check for process running with privileges
    if(getuid()) throw std::runtime_error("Pandora could not start without root user privileges. Run the process as root.");

    // Initial server configuration
    pandora::ServerOptions server_options(pandora::server_constants::default_port_number,
                                          pandora::server_constants::default_debug_enabled,
                                          pandora::server_constants::default_logs_enabled);

    // Server Session ID
    server_options.SetServerSessionID(pandora::server_utilities::GenerateServerSessionID());

    // Command-line arguments for server startup override
    // p={Port Number: #} d={Debug enabled: off} l={Logs enabled: off}
    std::vector<std::string> args(argv, argv + argc);
    server_options.OverrideOptions(args);

    // Server creation
    httpserver::webserver pandora_storage_server = httpserver::create_webserver(server_options.GetPortNumber())
                               .not_found_resource(pandora::server_endpoints::resource_not_found)
                               .method_not_allowed_resource(pandora::server_endpoints::method_not_allowed);

    // Storage Core: Main Cache creation
    std::shared_ptr<pandora::ElementContainerCache> main_cache = std::make_shared<pandora::ElementContainerCache>();

    // Server Endpoints Referencing
    pandora::server_utilities::SetEndpoints(pandora_storage_server, main_cache, server_options);
    
    // Start Pandora Storage Server in non-blocking mode
    pandora_storage_server.start(false);

    // Create Pandora's directories
    pandora::server_utilities::CreateBaseDirectories();

    // Initial Live Memory Filling
    main_cache->InitialLiveMemoryFilling();

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