// *************************************
// Pandora Storage Server
// Server
// 'main.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/core/main_data.h"
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
    pandora::ServerOptions server_options(pandora::constants::default_port_number,
                                          pandora::constants::default_debug_enabled,
                                          pandora::constants::default_logs_enabled);

    // Command-line arguments for server startup override
    // p={Port Number: #} d={Debug enabled: off} l={Logs enabled: off}
    std::vector<std::string> args(argv, argv + argc);
    server_options.OverrideOptions(args);

    // Non-Debug mode
    if(!server_options.GetDebugEnabled()) {

        // Daemonize process
        int daemon_result = daemon(1, 1);

        // Check for daemon result
        if(daemon_result == -1) {
            std::cout << "\n<!> Pandora Storage Server encountered an error while trying to daemonize the process for Non-Debug mode.\n\n" << std::endl;
            return -1;
        }

    }

    // Server creation
    httpserver::webserver pandora_storage_server = httpserver::create_webserver(server_options.GetPortNumber())
                               .not_found_resource(pandora::endpoints::resource_not_found)
                               .method_not_allowed_resource(pandora::endpoints::method_not_allowed);

    // Create Pandora's directories
    pandora::utilities::CreateBaseDirectories();

    // Storage Core: Main Cache creation
    std::shared_ptr<pandora::ElementContainerCache> main_cache = std::make_shared<pandora::ElementContainerCache>();
    // Storage Core: Main Data creation
    std::shared_ptr<pandora::MainData> main_data = std::make_shared<pandora::MainData>(main_cache, &server_options);

    // Server Endpoints Referencing
    pandora::utilities::SetEndpoints(pandora_storage_server, main_data);
    
    // Start Pandora Storage Server in non-blocking mode
    pandora_storage_server.start(false);

    // Server startup messages
    std::cout << "\n<<< Pandora Storage Server >>>\n\n";
    std::cout << "<> Debug mode: " << (server_options.GetDebugEnabled() ? std::string("Enabled") : std::string("Disabled - PID: " + std::to_string(getpid()))) << "\n";
    std::cout << "<> Logs: " << (server_options.GetLogsEnabled() ? std::string("Enabled") : std::string("Disabled")) << "\n";;
    std::cout << "<> Server Session ID: " << server_options.GetServerSessionID() << "\n";
    
    // Logs file
    if(server_options.GetLogsEnabled())
        std::cout << "<> Logs will be recorded at file: " << server_options.GetLogsFilePath() << "\n";

    std::cout << "\n* Running on port: " << server_options.GetPortNumber() << "\n\n";

    // Run forever
    for(;;) {}
    
    return 0;

}