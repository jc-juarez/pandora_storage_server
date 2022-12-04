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

        inline constexpr int port_number {5000};
        inline constexpr bool debug_enabled {true};
        inline constexpr bool logs_enabled {true};
        inline constexpr bool display_requests {true};

    }

    namespace server_constants {

        inline constexpr char port_number_option {'p'};
        inline constexpr char debug_enabled_option {'d'};
        inline constexpr char logs_enabled_option {'l'};
        inline constexpr char display_requests_option {'r'};
        inline constexpr std::string_view off_option {"off"};
        inline constexpr std::string_view pandora_directory_path {"/var/lib/pandora"};
        inline constexpr std::string_view storage_directory_path {"/var/lib/pandora/storage"};
        inline constexpr std::string_view logs_directory_path {"/var/lib/pandora/logs"};
        inline constexpr std::string_view elements_directory_path {"/var/lib/pandora/storage/elements"};
        inline constexpr std::string_view files_directory_path {"/var/lib/pandora/storage/files"};

    }

} // namespace pandora

#endif