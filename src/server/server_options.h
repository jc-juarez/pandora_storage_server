// *************************************
// Pandora Storage Server
// Server
// 'server_options.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H

#include <string>
#include <vector>

namespace pandora {

    class ServerOptions {

        // Constructor
        public:
            ServerOptions(int, bool, bool, std::string, std::string);

        // Private properties
        private:
            int m_port_number;
            bool m_debug_enabled;
            bool m_logs_enabled;
            std::string m_server_session_id;
            std::string m_logs_file_path;

        // Setter Methods
        public:
            void SetPortNumber(int);
            void SetDebugEnabled(bool);
            void SetLogsEnabled(bool);
            void SetServerSessionID(std::string);
            void SetLogsFilePath(std::string);

        // Getter Methods
        public:
            int GetPortNumber() const;
            bool GetDebugEnabled() const;
            bool GetLogsEnabled() const;
            std::string GetServerSessionID() const;
            std::string GetLogsFilePath() const;

        // Server Capabilities
        public:
            void OverrideOptions(const std::vector<std::string>);
            void ConsoleLog(const std::string);
            void CreateLogsFile();

    };

} // namespace pandora

#endif