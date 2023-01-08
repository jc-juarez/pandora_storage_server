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
#include <chrono>

namespace pandora {

        // Constructor
        ServerOptions::ServerOptions(int port_number, bool debug_enabled, bool logs_enabled) {

            m_port_number = port_number;
            m_debug_enabled = debug_enabled;
            m_logs_enabled = logs_enabled;
            m_server_session_id = "";
            m_logs_file_path = "";

            // Generate Server Session Identifier
            GenerateServerSessionID();

        }

        void ServerOptions::GenerateServerSessionID() {

            // Server session ID (8 first digits correspond to date, 9 latter digits are randomly generated)
            // Date-Time Identifier
            pandora::utilities::DateTime date_time = pandora::utilities::GetDateTime();
            m_server_session_id.append(date_time.year + 
                                     date_time.month +
                                     date_time.day + "-");
            // Random identifier
            m_server_session_id.append(pandora::utilities::GetRandomString_Size9());

        }

        // Setter Methods

        void ServerOptions::SetPortNumber(int port_number) { m_port_number = port_number; }

        void ServerOptions::SetDebugEnabled(bool debug_enabled) { m_debug_enabled = debug_enabled; }

        void ServerOptions::SetLogsEnabled(bool logs_enabled) { m_logs_enabled = logs_enabled; }

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

                // Ignore keywords commands
                if(command == pandora::constants::pandora || command == pandora::constants::sudo) continue;

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

            // Create Logs file for Server Session if option selected
            if(GetLogsEnabled()) CreateLogsFile();
            
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
            pandora::storage::FileOperation(GetLogsFilePath(), pandora::constants::FileOption::Create);
        }

        void ServerOptions::LogToFile(pandora::utilities::TransactionData& transaction_data) {
            std::string log_content {transaction_data.logs_stream.str()};
            std::thread write_logs_thread(&ServerOptions::LogToFileThread, this, log_content);
            write_logs_thread.detach();
        }

        void ServerOptions::LogToFileThread(const std::string log_content) {
            write_logs_mutex.lock();
            try {
                pandora::storage::FileOperation(GetLogsFilePath(), pandora::constants::FileOption::Append, log_content);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not write logs to the '" + GetLogsFilePath() + "' logs file.");
                throw std::runtime_error(error);
            }
            write_logs_mutex.unlock();
        }

        void ServerOptions::LogTransactionStartedFinished(int server_code, pandora::utilities::TransactionData& transaction_data, const std::string ellapsed_milliseconds) {
            std::string log {};
            log.append("[" + transaction_data.transaction_id + "] ");
            log.append(server_code == constants::TransactionStartedCode ? "Started <1> " : "Finished <0> ");
            if(server_code == pandora::constants::TransactionFinishedCode) log.append("{Ellapsed time: " + ellapsed_milliseconds + " ms} ");
            log.append(pandora::utilities::GetDateTimeString() + " -> " + transaction_data.http_method + " " + transaction_data.request_path);
            DebugLog(log, transaction_data.logs_stream);
        }

        void ServerOptions::LogInfo(pandora::utilities::TransactionData& transaction_data) {
            std::string log {};
            log.append("[" + transaction_data.transaction_id + "] ");
            log.append("Info <" + std::to_string(pandora::constants::TransactionInfoCode) + "> " + pandora::utilities::GetDateTimeString() + " -> " + transaction_data.log);
            DebugLog(log, transaction_data.logs_stream);
            transaction_data.log.clear();
        }

        void ServerOptions::LogError(int error_code, pandora::utilities::TransactionData& transaction_data) {

            auto stop = std::chrono::high_resolution_clock::now();
            std::string ellapsed_milliseconds = pandora::utilities::GetEllapsedMillisecondsString(transaction_data.start_time_point, stop);
            transaction_data.ellapsed_milliseconds = ellapsed_milliseconds;

            std::string log {};
            log.append("[" + transaction_data.transaction_id + "] ");

            log.append("Error <" + std::to_string(error_code) + "> " + "{Ellapsed time: " + transaction_data.ellapsed_milliseconds + " ms} " + 
                        pandora::utilities::GetDateTimeString() + " -> " + transaction_data.log);

            DebugLog(log, transaction_data.logs_stream);
            LogToFile(transaction_data);
            std::string error {};
            error.append("Error <" + std::to_string(error_code) + "> | " + transaction_data.log);
            transaction_data.log.clear();
            throw std::runtime_error(error);

        }

} // namespace pandora