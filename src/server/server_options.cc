// *************************************
// Pandora Storage Server
// Server
// 'server_options.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../storage/storage.h"
#include "server_utilities.h"
#include "server_options.h"
#include "server_constants.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <vector>

namespace pandora {

        // Constructor
        ServerOptions::ServerOptions(int port_number, bool debug_enabled, bool logs_enabled) {
            m_port_number = port_number;
            m_debug_enabled = debug_enabled;
            m_logs_enabled = logs_enabled;
            m_server_session_id = "";
            m_logs_file_path = "";
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
                    case pandora::constants::port_number_option :
                        SetPortNumber(std::stoi(option_value));
                        break;
                    case pandora::constants::debug_enabled_option :
                        if(option_value == pandora::constants::off_option) SetDebugEnabled(false);
                        break;
                    case pandora::constants::logs_enabled_option :
                        if(option_value == pandora::constants::off_option) SetLogsEnabled(false);
                        break;
                }

            }
            
        }

        void ServerOptions::ConsoleLog(const std::string log) { if(GetDebugEnabled()) std::cout << "<> " << log << "\n"; }

        void ServerOptions::AppendLog(const std::string log, std::stringstream& logs_stream) { if(GetLogsEnabled()) logs_stream << log << std::endl; }

        void ServerOptions::DebugLog(const std::string log, std::stringstream& logs_stream) {
            ConsoleLog(log);
            AppendLog(log, logs_stream);
        }

        void ServerOptions::CreateLogsFile() {
            std::string logs_file_path {};
            logs_file_path.append(pandora::constants::logs_directory_path + "/pandoralog-" + GetServerSessionID() + ".txt");
            SetLogsFilePath(logs_file_path);
            pandora::storage::AddFileContent(GetLogsFilePath(), "", false);
        }

        void ServerOptions::LogToFile(pandora::utilities::RequestData& request_data) {
            std::string log_content {request_data.logs_stream.str()};
            std::thread write_logs_thread(&ServerOptions::LogToFileThread, this, log_content);
            write_logs_thread.detach();
        }

        void ServerOptions::LogToFileThread(const std::string log_content) {
            write_logs_mutex.lock();
            try {
                pandora::storage::AddFileContent(GetLogsFilePath(), log_content, true);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not write logs to the '" + GetLogsFilePath() + "' logs file.");
                throw std::runtime_error(error);
            }
            write_logs_mutex.unlock();
        }

        void ServerOptions::LogTransactionStartedFinished(int server_code, pandora::utilities::RequestData& request_data) {
            std::string log {};
            log.append("[" + request_data.transaction_id + "] ");
            log.append(server_code == constants::TransactionStartedCode ? "Started (1) " : "Finished (0) ");
            log.append(pandora::utilities::GetDateTimeString() + " -> " + request_data.http_method + " " + request_data.request_path);
            DebugLog(log, request_data.logs_stream);
        }

        void ServerOptions::LogInfo(pandora::utilities::RequestData& request_data) {
            std::string log {};
            log.append("[" + request_data.transaction_id + "] ");
            log.append("Info (" + std::to_string(pandora::constants::TransactionInfoCode) + ") " + pandora::utilities::GetDateTimeString() + " -> " + request_data.log);
            DebugLog(log, request_data.logs_stream);
        }

        void ServerOptions::LogError(int error_code, pandora::utilities::RequestData& request_data) {
            std::string log {};
            log.append("[" + request_data.transaction_id + "] ");
            log.append("Error (" + std::to_string(error_code) + ") " + pandora::utilities::GetDateTimeString() + " -> " + request_data.log);
            DebugLog(log, request_data.logs_stream);
            LogToFile(request_data);
            std::string error {};
            error.append(std::to_string(error_code) + "-Error " + "[" + request_data.transaction_id + "]" + ": " + request_data.log);
            throw std::runtime_error(error);
        }

} // namespace pandora