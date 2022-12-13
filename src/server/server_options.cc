// *************************************
// Pandora Storage Server
// Server
// 'server_options.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/storage_operations.h"
#include "server_options.h"
#include "server_constants.h"
#include <iostream>
#include <string>
#include <vector>

namespace pandora {

        // Constructor
        ServerOptions::ServerOptions(int port_number, bool debug_enabled, bool logs_enabled, std::string server_session_id, std::string logs_file_path) {
            m_port_number = port_number;
            m_debug_enabled = debug_enabled;
            m_logs_enabled = logs_enabled;
            m_server_session_id = server_session_id;
            m_logs_file_path = logs_file_path;
        }

        // Setter Methods

        void ServerOptions::SetPortNumber(int port_number) { m_port_number = port_number; }

        void ServerOptions::SetDebugEnabled(bool debug_enabled) { m_debug_enabled = debug_enabled; }

        void ServerOptions::SetLogsEnabled(bool logs_enabled) { m_logs_enabled = logs_enabled; }

        void ServerOptions::SetServerSessionID(std::string server_session_id) { m_server_session_id = server_session_id; }

        void ServerOptions::SetLogsFilePath(std::string logs_file_path) { m_logs_file_path = logs_file_path; }

        // Getter Methods

        int ServerOptions::GetPortNumber() const { return m_port_number; }

        bool ServerOptions::GetDebugEnabled() const { return m_debug_enabled; }

        bool ServerOptions::GetLogsEnabled() const { return m_logs_enabled; }

        std::string ServerOptions::GetServerSessionID() const { return m_server_session_id; }

        std::string ServerOptions::GetLogsFilePath() const { return m_logs_file_path; }

        // Server Capabilities

        void ServerOptions::OverrideOptions(const std::vector<std::string> args) {

            if(args.empty()) return;

            // Options updates based on commands
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
                        SetPortNumber(std::stoi(option_value));
                        break;
                    case pandora::server_constants::debug_enabled_option :
                        if(option_value == pandora::server_constants::off_option) SetDebugEnabled(false);
                        break;
                    case pandora::server_constants::logs_enabled_option :
                        if(option_value == pandora::server_constants::off_option) SetLogsEnabled(false);
                        break;
                }

            }
            
        }

        void ServerOptions::ConsoleLog(const std::string log) { if(GetDebugEnabled()) std::cout << "<> " << log << "\n"; }

        void ServerOptions::CreateLogsFile() {
            std::string logs_file_path {};
            logs_file_path.append(std::string(pandora::server_constants::logs_directory_path) + "/pandoralog-" + GetServerSessionID() + ".txt");
            SetLogsFilePath(logs_file_path);
            pandora::storage::AddFileContent(GetLogsFilePath(), "", false);
            ConsoleLog("Logs will be recorded at file: " + GetLogsFilePath());
        }

} // namespace pandora