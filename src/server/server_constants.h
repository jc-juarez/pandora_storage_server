// *************************************
// Pandora Storage Server
// Server
// 'server_constants.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

#include <string>

namespace pandora {

    namespace server_constants {

        // Default options
        inline constexpr int default_port_number {5000};
        inline constexpr bool default_debug_enabled {true};
        inline constexpr bool default_logs_enabled {true};

        // General constants
        inline constexpr char port_number_option {'p'};
        inline constexpr char debug_enabled_option {'d'};
        inline constexpr char logs_enabled_option {'l'};
        inline constexpr char display_requests_option {'r'};
        inline const std::string off_option {"off"};
        inline const std::string http_put {"PUT"};
        inline const std::string http_post {"POST"};
        inline const std::string http_get {"GET"};
        inline const std::string http_delete {"DELETE"};
        inline const std::string pandora_directory_path {"/var/lib/pandora"};
        inline const std::string storage_directory_path {"/var/lib/pandora/storage"};
        inline const std::string logs_directory_path {"/var/lib/pandora/logs"};
        inline const std::string elements_directory_path {"/var/lib/pandora/storage/elements"};
        inline const std::string files_directory_path {"/var/lib/pandora/storage/files"};

        // Endpoints URLs
        // Elements Container
        inline const std::string create_elements_container_endpoint_url {"/pandora/create-elements-container/{arg1}"};
        inline const std::string delete_elements_container_endpoint_url {"/pandora/delete-elements-container/{arg1}"};
        // Elements
        inline const std::string set_element_endpoint_url {"/pandora/set-element/{arg1}/{arg2}"};
        inline const std::string get_element_endpoint_url {"/pandora/get-element/{arg1}/{arg2}"};
        inline const std::string delete_element_endpoint_url {"/pandora/delete-element/{arg1}/{arg2}"};

    }

} // namespace pandora

#endif