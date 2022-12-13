// *************************************
// Pandora Storage Server
// Server
// 'elements_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "../storage_operations.h"
#include "orion_engine.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementsContainer(const std::string& elements_container_name, const std::string& transaction_id, std::stringstream& logs_stream) {
    
            // Live Memory

            // Check for containers with the same name
            delete_elements_container_mutex.lock_shared();

            std::string elements_container_path {std::string(server_constants::elements_directory_path)};
            elements_container_path.append("/" + elements_container_name);

            for (const auto & entry : std::filesystem::directory_iterator(std::string(server_constants::elements_directory_path))) {
                if(std::string(entry.path()) == elements_container_path) {
                    std::cout << "Elements Container already exists\n";
                    throw std::runtime_error("");
                }
            }
            
            storage::AddFileContent(elements_container_path, "", false);

            delete_elements_container_mutex.unlock_shared();
        }

    }

} // namespace pandora