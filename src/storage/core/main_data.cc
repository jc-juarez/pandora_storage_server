// *************************************
// Pandora Storage Server
// Storage Core
// 'main_data.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "../../server/server_options.h"
#include "live-memory/element_container_cache.h"
#include "element_container.h"
#include "main_data.h"
#include <unordered_map>
#include <filesystem>
#include <utility>
#include <memory>
#include <string>

namespace pandora {

    MainData::MainData(std::shared_ptr<ElementContainerCache>& main_cache, ServerOptions* server_options) {

        m_main_cache = main_cache;
        m_server_options = server_options;

    }

    ServerOptions* MainData::GetServerOptions() { return m_server_options; };

    std::shared_ptr<ElementContainerCache>& MainData::GetMainCache() { return m_main_cache; };

    ElementContainer& MainData::GetElementContainer(const std::string& element_container_name) { return m_element_containers.find(element_container_name)->second; };

    void MainData::AddElementContainer(const std::string& element_container_name) {

        m_element_containers.emplace(element_container_name, element_container_name);

    }

    void MainData::LockExclusiveElementContainerOperations() { m_element_container_operations_mutex.lock(); }
    
    void MainData::UnlockExclusiveElementContainerOperations() { m_element_container_operations_mutex.unlock(); }

    bool MainData::ElementContainerExists(const std::string& element_container_name) { 

        // Element Container Path
        std::string element_container_path {pandora::constants::element_containers_directory_path};
        element_container_path.append("/" + element_container_name);

        // Check if Element Container exists 
        return std::filesystem::exists(element_container_path) && m_element_containers.count(element_container_name);
            
    }

} // namespace pandora