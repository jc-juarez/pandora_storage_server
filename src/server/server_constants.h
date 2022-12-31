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
        inline constexpr int http_ok {200};
        inline constexpr int http_bad_request {400};
        inline constexpr int http_internal_error {500};
        inline constexpr int element_container_size_index {1};
        inline constexpr int not_found_int {-1};
        inline constexpr char port_number_option {'p'};
        inline constexpr char debug_enabled_option {'d'};
        inline constexpr char logs_enabled_option {'l'};
        inline const std::string not_found_string {""};
        inline const std::string off_option {"off"};
        inline const std::string http_put {"PUT"};
        inline const std::string http_post {"POST"};
        inline const std::string http_get {"GET"};
        inline const std::string http_delete {"DELETE"};
        inline const std::string element_container_name {"element_container_name"};
        inline const std::string element_id {"element_id"};
        inline const std::string element_value {"element_value"};
        inline const std::string storage {"storage"};
        inline const std::string data {"data"};
        inline const std::string pandora_directory_path {"/var/lib/pandora"};
        inline const std::string logs_directory_path {"/var/lib/pandora/logs"};
        inline const std::string element_containers_directory_path {"/var/lib/pandora/element-containers"};

        // Validation and Restrictions
        inline constexpr size_t ElementContainerNameMaxSize {100};
        inline constexpr size_t ElementContainerMaxCapacity {50000};
        inline constexpr size_t ElementIDMaxSize {50};
        inline constexpr size_t ElementValueMaxSize {1000};
        inline const std::string element_delimeter {"<//>"};

        // Server Codes
        // Regular Codes
        inline constexpr int TransactionStartedCode {0};
        inline constexpr int TransactionFinishedCode {1};
        inline constexpr int TransactionInfoCode {100};
        //Error Codes
        inline constexpr int ElementContainerExistsErrorCode {900};
        inline constexpr int ElementContainerNotExistsErrorCode {901};
        inline constexpr int ElementContainerFull {902};
        inline constexpr int ElementNotExists {915};
        inline constexpr int ParameterOversizeErrorCode {930};
        inline constexpr int ParameterEmptyErrorCode {931};
        inline constexpr int ElementStorageError {960};

        // Exception Messages
        inline const std::string element_container_not_found {"element_container_not_found"};
        inline const std::string element_container_data_file_not_found {"element_container_data_file_not_found"};
        inline const std::string element_container_storage_file_not_found {"element_container_storage_file_not_found"};

        // Endpoints URLs
        // Element Container
        inline const std::string create_element_container_endpoint_url {"/pandora/create-element-container/{element_container_name}"};
        inline const std::string delete_element_container_endpoint_url {"/pandora/delete-element-container/{element_container_name}"};
        // Elements
        inline const std::string set_element_endpoint_url {"/pandora/set-element/{element_container_name}/{element_id}/{element_value}"};
        inline const std::string get_element_endpoint_url {"/pandora/get-element/{element_container_name}/{element_id}"};
        inline const std::string delete_element_endpoint_url {"/pandora/delete-element/{element_container_name}/{element_id}"};

    }

} // namespace pandora

#endif