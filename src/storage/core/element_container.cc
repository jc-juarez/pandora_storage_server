// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "element_container.h"
#include <unordered_map>
#include <filesystem>
#include <string>
#include <mutex>

namespace pandora {

    ElementContainer::ElementContainer(const std::string& element_container_name) {

        m_element_container_name = element_container_name;
        m_element_container_size = 0;

        m_element_container_path.append(pandora::constants::element_containers_directory_path + "/" + GetElementContainerName());
        m_element_container_data_file_path.append(GetElementContainerPath() + "/" + pandora::constants::data);
        m_element_container_storage_file_path.append(GetElementContainerPath() + "/" + pandora::constants::storage);

        // If true, Element Container already exists on Disk and is being brought to Memory on server startup
        if(std::filesystem::exists(GetElementContainerDataFilePath())) {
            // Recover Element Container Data
        }

    }

    std::string ElementContainer::GetElementContainerName() const { return m_element_container_name; }

    int ElementContainer::GetElementContainerSize() const { return m_element_container_size; }

    std::string ElementContainer::GetElementContainerPath() const { return m_element_container_path; }

    std::string ElementContainer::GetElementContainerDataFilePath() const { return m_element_container_data_file_path; }

    std::string ElementContainer::GetElementContainerStorageFilePath() const { return m_element_container_storage_file_path; }

    void ElementContainer::UpdateElementContainerSize(int) {}

    void ElementContainer::LockExclusiveElementOperations() { m_element_container_lock.lock(); }

    void ElementContainer::UnlockExclusiveElementOperations() { m_element_container_lock.unlock(); }

} // namespace pandora