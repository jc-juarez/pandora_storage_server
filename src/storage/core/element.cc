// *************************************
// Pandora Storage Server
// Storage Core
// 'element.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "live-memory/element_container_cache.h"
#include <filesystem>

namespace pandora {

    namespace core {

        void SetElement(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {

            // Set Element on Disk
            pandora::ElementContainerCache::delete_element_container_mutex.lock_shared();

            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            if(!std::filesystem::exists(element_container_path)) {
                pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not exist and Element '" +
                                         request_data.arguments[pandora::constants::element_id] + "' could not be set.");
                server_options->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Missing replace existing Element value
            
            std::string element {};

            // Append Element ID
            element.append(request_data.arguments[pandora::constants::element_id]);
            // Append Delimeter
            element.append(pandora::constants::element_delimeter);
            // Append Value
            element.append(request_data.arguments[pandora::constants::element_value] + "\n");

            pandora::storage::AddFileContent(element_container_path, element, true);

            request_data.log.append("Element '" + request_data.arguments[pandora::constants::element_id] + "' has been set inside the Element Container '" +
                                     request_data.arguments[pandora::constants::element_container_name] + "'.");
            server_options->LogInfo(request_data);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();

            // Set Live Memory

        }

    }

} // namespace pandora