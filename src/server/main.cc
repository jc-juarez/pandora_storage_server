// *************************************
// Pandora Storage Server
// Server
// 'main.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************
      
#include "server_utilities.h"
#include "server_constants.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// External server options definition
namespace pandora {

    namespace server_options {

        int port_number {};
        bool debug_enabled {};
        bool logs_enabled {};
        bool display_requests {};

    }

} // namespace pandora

void HelloWorld() {
    for(;;) {
        std::cout << "Hellooooo!!!\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

class hello_world_resource : public httpserver::http_resource {
public:
    std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&) {
        std::cout << "Called\n";
        if(pandora::server_options::debug_enabled) std::cout << "IT WORKS Debug enabled after";
        std::thread t1(HelloWorld);
        t1.detach();
        std::string response {"Hello World from "};
        response.append(std::to_string(pandora::server_options::port_number));
        return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(static_cast<httpserver::string_response>(response)));
    }
};

int main(int argc, char** argv) {

    // Initial server configiration
    pandora::server_options::port_number = pandora::default_options::port_number;
    pandora::server_options::debug_enabled = pandora::default_options::debug_enabled;
    pandora::server_options::logs_enabled = pandora::default_options::logs_enabled;
    pandora::server_options::display_requests = pandora::default_options::display_requests;
    
    // Command-line arguments for server startup
    // p={Port Number: #} d={Debug enabled: off} l={Logs enabled: off} r={Display requests: off}
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
                case pandora::server_constants::display_requests_option :
                    if(option_value == pandora::server_constants::off_option) pandora::server_options::display_requests = false;
                    break;
            }
        }
    }

    // Server startup messages
    std::cout << "\n<<< Pandora Storage Server >>>\n\n";
    pandora::server_utilities::ConsoleLog("Debug mode: " + (pandora::server_options::debug_enabled ? std::string("Enabled") : std::string("Disabled")));
    pandora::server_utilities::ConsoleLog("Logs: " + (pandora::server_options::logs_enabled ? std::string("Enabled") : std::string("Disabled")));
    pandora::server_utilities::ConsoleLog("Display Requests: " + (pandora::server_options::display_requests ? std::string("Enabled") : std::string("Disabled")));
    if(pandora::server_options::debug_enabled) std::cout << "\n";
    std::cout << "* Running on port: " << pandora::server_options::port_number << "\n";

    // Web server creation
    httpserver::webserver ws = httpserver::create_webserver(pandora::server_options::port_number);

    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr);

    ws.start(true);
    
    return 0;
}