// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

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

        void CreateElementContainer(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::server_utilities::RequestData& request_data) {
    
            // Live Memory
            
            // Check for element container with same name
            pandora::ElementContainerCache::delete_element_container_mutex.lock_shared();

            std::string element_container_path {pandora::server_constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::server_constants::element_container_name]);

            for (const auto & entry : std::filesystem::directory_iterator(pandora::server_constants::element_containers_directory_path)) {
                if(std::string(entry.path()) == element_container_path) {
                    server_options->DebugLog("Element Container already exists.", request_data.logs_stream);
                    server_options->LogToFile(request_data.logs_stream.str());
                    throw std::runtime_error("");
                }
            }
            
            storage::AddFileContent(element_container_path, "", false);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
        }

    }

} // namespace pandora