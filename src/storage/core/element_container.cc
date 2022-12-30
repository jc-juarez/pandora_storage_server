// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "live-memory/element_container_cache.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    namespace core {

        std::string GetElementContainerData(int index, const std::string& element_container_data_path) {

            std::ifstream element_container_data_file;
            element_container_data_file.open(element_container_data_path);

            std::string line {};
            int element_container_current_size {};
            int current_index {1};

            while (std::getline(element_container_data_file, line)) {
                if(current_index == index) {
                    return line;
                }
                ++current_index;
            }

            return pandora::constants::not_found_string;
        }

        void CreateElementContainer(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            // Lock shared operation
            main_cache->LockSharedDeleteElementContainerOperation();

            // Element Container Path
            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            // Check if Element Container already exists
            if(std::filesystem::exists(element_container_path)) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' already exists.");
                server_options->LogError(pandora::constants::ElementContainerExistsErrorCode, request_data);
            }
            
            // Create Element Container
            pandora::storage::CreateDirectory(element_container_path);

            // Storage File creation
            std::string element_container_storage_path {element_container_path};
            element_container_storage_path.append("/" + pandora::constants::storage);
            pandora::storage::AddFileContent(element_container_storage_path, "", false);
            request_data.log.append("Element Container Storage File was created succesfully.");
            server_options->LogInfo(request_data);

            // Data File creation
            std::string element_container_data_path {element_container_path};
            element_container_data_path.append("/" + pandora::constants::data);
            pandora::storage::AddFileContent(element_container_data_path, "0\n", true);
            request_data.log.append("Element Container Data File was created successfully.");
            server_options->LogInfo(request_data);

            // Log Element Container successful creation
            request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was created succesfully.");
            server_options->LogInfo(request_data);

            // Unlock shared operation
            main_cache->UnlockSharedDeleteElementContainerOperation();
        }

        void DeleteElementContainer(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            // Lock exclusive operation
            main_cache->LockExclusiveDeleteElementContainerOperation();

            // Element Container path
            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            // Check if Element Container does not exist
            if(!std::filesystem::exists(element_container_path)) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not exist and could not be deleted.");
                server_options->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Delete Element Container
            std::filesystem::remove_all(element_container_path);
            
            // Log Element Container successful deletion
            request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was deleted succesfully.");
            server_options->LogInfo(request_data);

            // Unlock exclusive operation
            main_cache->UnlockExclusiveDeleteElementContainerOperation();
        }

    }

} // namespace pandora