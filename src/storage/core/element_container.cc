// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "live-memory/element_container_cache.h"
#include "../storage.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(const std::string& elements_container_name, const std::string& transaction_id, std::stringstream& logs_stream) {
    
            // Live Memory
            
            // Check for containers with the same name
            pandora::ElementContainerCache::delete_element_container_mutex.lock_shared();

            std::string elements_container_path {server_constants::element_containers_directory_path};
            elements_container_path.append("/" + elements_container_name);

            for (const auto & entry : std::filesystem::directory_iterator(server_constants::element_containers_directory_path)) {
                if(std::string(entry.path()) == elements_container_path) {
                    std::cout << "Element Container already exists\n";
                    throw std::runtime_error("");
                }
            }
            
            storage::AddFileContent(elements_container_path, "", false);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
        }

    }

} // namespace pandora