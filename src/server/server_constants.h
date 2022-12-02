// *************************************
// Pandora Storage Server
// Server
// 'server_constants.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

#include <string_view>

namespace pandora {

    namespace default_options {

        constexpr int port_number {5000};
        constexpr bool debug_enabled {true};
        constexpr bool logs_enabled {true};
        constexpr bool display_requests {true};

    }

    namespace server_constants {

        constexpr char port_number_option {'p'};
        constexpr char debug_enabled_option {'d'};
        constexpr char logs_enabled_option {'l'};
        constexpr char display_requests_option {'r'};
        constexpr std::string_view off_option {"off"};

    }

} // namespace pandora

#endif