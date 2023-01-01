// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "../storage.h"
#include "element_container.h"
#include <filesystem>
#include <fstream>
#include <string>

namespace pandora {

    ElementContainer::ElementContainer(const std::string& element_container_name) {

        m_element_container_name = element_container_name;
        m_element_container_size = 0;

        m_element_container_path.append(pandora::constants::element_containers_directory_path + "/" + GetElementContainerName());
        m_element_container_data_file_path.append(GetElementContainerPath() + "/" + pandora::constants::data);
        m_element_container_storage_file_path.append(GetElementContainerPath() + "/" + pandora::constants::storage);

        // If true, Element Container already exists on Disk and is being brought to Memory on server startup
        if(std::filesystem::exists(GetElementContainerPath())) {
            // Recover Element Container size
            m_element_container_size = std::stoi(GetElementContainerData(pandora::constants::element_container_size_index));
            return;
        }

        // Construct Element Container
        pandora::storage::CreateDirectory(GetElementContainerPath());

        // Data File creation
        pandora::storage::AddFileContent(GetElementContainerDataFilePath());
        // Storage File creation
        pandora::storage::AddFileContent(GetElementContainerStorageFilePath());

        // Fill Data File
        pandora::storage::AddFileContent(GetElementContainerDataFilePath(), std::to_string(GetElementContainerSize()) + "\n", true);
        pandora::storage::AddFileContent(GetElementContainerDataFilePath(), GetElementContainerName() + "\n", true);
        pandora::storage::AddFileContent(GetElementContainerDataFilePath(), GetElementContainerPath() + "\n", true);
        pandora::storage::AddFileContent(GetElementContainerDataFilePath(), GetElementContainerDataFilePath() + "\n", true);
        pandora::storage::AddFileContent(GetElementContainerDataFilePath(), GetElementContainerStorageFilePath() + "\n", true);

    }

    std::string ElementContainer::GetElementContainerName() const { return m_element_container_name; }

    int ElementContainer::GetElementContainerSize() const { return m_element_container_size; }

    std::string ElementContainer::GetElementContainerPath() const { return m_element_container_path; }

    std::string ElementContainer::GetElementContainerDataFilePath() const { return m_element_container_data_file_path; }

    std::string ElementContainer::GetElementContainerStorageFilePath() const { return m_element_container_storage_file_path; }

    std::string ElementContainer::GetElementContainerData(int data_index) {

        std::ifstream element_container_data_file;
        element_container_data_file.open(GetElementContainerDataFilePath());

        std::string line {};
        int current_index {0};

        while (std::getline(element_container_data_file, line)) {
            if(current_index == data_index) {
                return line;
            }
            ++current_index;
        }

        return pandora::constants::not_found_string;

    }

    void ElementContainer::UpdateElementContainerSize(int size) {
        
        pandora::storage::ReplaceFileLine(pandora::constants::element_container_size_index + 1, GetElementContainerDataFilePath(), std::to_string(size));
        m_element_container_size = size;

    }

    void ElementContainer::LockExclusiveElementOperations() { m_element_container_lock.lock(); }

    void ElementContainer::UnlockExclusiveElementOperations() { m_element_container_lock.unlock(); }

} // namespace pandora