// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "server_options.h"
#include <iostream>
#include <string>

namespace pandora {

    namespace server_utilities {

        void ConsoleLog(const std::string log) {
            if(server_options::debug_enabled) std::cout << "<> " << log << "\n";
        }

    }

} // namespace pandora