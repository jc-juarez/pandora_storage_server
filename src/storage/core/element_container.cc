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
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            pandora::ElementContainerCache::delete_element_container_mutex.lock_shared();

            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            if(std::filesystem::exists(element_container_path)) {
                pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' already exists.");
                server_options->LogError(pandora::constants::ElementContainerExistsErrorCode, request_data);
            }
            
            storage::AddFileContent(element_container_path, "", false);

            request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was created succesfully.");
            server_options->LogInfo(request_data);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
        }

        void DeleteElementContainer(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            pandora::ElementContainerCache::delete_element_container_mutex.lock();

            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            if(!std::filesystem::exists(element_container_path)) {
                pandora::ElementContainerCache::delete_element_container_mutex.unlock();
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not exist and could not be deleted.");
                server_options->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            std::filesystem::remove(element_container_path);
            
            request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was deleted succesfully.");
            server_options->LogInfo(request_data);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock();
        }

    }

} // namespace pandora