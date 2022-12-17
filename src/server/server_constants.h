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

    namespace constants {

        // Default options
        inline constexpr int default_port_number {5000};
        inline constexpr bool default_debug_enabled {true};
        inline constexpr bool default_logs_enabled {true};

        // General constants
        inline constexpr char port_number_option {'p'};
        inline constexpr char debug_enabled_option {'d'};
        inline constexpr char logs_enabled_option {'l'};
        inline const std::string off_option {"off"};
        inline const std::string http_put {"PUT"};
        inline const std::string http_post {"POST"};
        inline const std::string http_get {"GET"};
        inline const std::string http_delete {"DELETE"};
        inline const std::string element_container_name {"element_container_name"};
        inline const std::string element_id {"element_id"};
        inline const std::string pandora_directory_path {"/var/lib/pandora"};
        inline const std::string storage_directory_path {"/var/lib/pandora/storage"};
        inline const std::string logs_directory_path {"/var/lib/pandora/logs"};
        inline const std::string element_containers_directory_path {"/var/lib/pandora/storage/element-containers"};

        // Server Codes
        // Regular Codes
        inline constexpr int TransactionStartedCode {0};
        inline constexpr int TransactionFinishedCode {1};
        inline constexpr int TransactionInfoCode {100};
        //Error Codes
        inline constexpr int ElementContainerExistsErrorCode {900};
        inline constexpr int ElementContainerNotExistsErrorCode {901};

        // Endpoints URLs
        // Element Container
        inline const std::string create_element_container_endpoint_url {"/pandora/create-element-container/{element_container_name}"};
        inline const std::string delete_element_container_endpoint_url {"/pandora/delete-element-container/{element_container_name}"};
        // Elements
        inline const std::string set_element_endpoint_url {"/pandora/set-element/{element_container_name}/{element_id}"};
        inline const std::string get_element_endpoint_url {"/pandora/get-element/{element_container_name}/{element_id}"};
        inline const std::string delete_element_endpoint_url {"/pandora/delete-element/{element_container_name}/{element_id}"};

    }

} // namespace pandora

#endif