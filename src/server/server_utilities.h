// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_UTILITIES_H
#define SERVER_UTILITIES_H

#include <filesystem>
#include <string>

namespace pandora {

    namespace server_utilities {

        struct DateTime {
            std::string year {};
            std::string month {};
            std::string day {};
            std::string hours {};
            std::string minutes {};
            std::string seconds {};
        };

        void ConsoleLog(const std::string);
        void CreateDirectory(const std::filesystem::path);
        DateTime GetDateTime();

    }

} // namespace pandora

#endif
