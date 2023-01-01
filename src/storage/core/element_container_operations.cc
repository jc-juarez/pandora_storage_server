// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container_operations.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "element_container.h"
#include "main_data.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::RequestData& request_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            // Lock shared operation
            main_data->LockExclusiveElementContainerOperations();

            // Check if Element Container already exists
            if(main_data->ElementContainerExists(request_data.arguments[pandora::constants::element_container_name])) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' already exists.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerExistsErrorCode, request_data);
            }
            
            // Create Element Container
            main_data->AddElementContainer(request_data.arguments[pandora::constants::element_container_name]);

            // Log Element Container successful creation
            request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' was created succesfully.");
            main_data->GetServerOptions()->LogInfo(request_data);

            // Unlock shared operation
            main_data->UnlockExclusiveElementContainerOperations();

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