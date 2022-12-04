// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "server_options.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace pandora {

    namespace server_utilities {

        void ConsoleLog(const std::string log) {
            if(server_options::debug_enabled) std::cout << "<> " << log << "\n";
        }

        void CreateDirectory(const std::string path) {
            try {
                // Create Pandora Storage Server directory
                std::filesystem::create_directories(path);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not create the " + path + " directory.");
                throw std::runtime_error(error);
            }
        }

    }

} // namespace pandora