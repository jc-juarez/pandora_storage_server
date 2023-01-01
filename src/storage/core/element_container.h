// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_CONTAINER_H
#define ELEMENT_CONTAINER_H

#include <unordered_map>
#include <string>
#include <mutex>

namespace pandora {

    class ElementContainer {

        // Constructor
        public:
            ElementContainer(const std::string&);

        // No Copy Constructor
        public:
            ElementContainer(const ElementContainer&) = delete;

        // Properties
        private:
            // General properties
            std::string m_element_container_name;
            int m_element_container_size;
            // Paths
            std::string m_element_container_path;
            std::string m_element_container_data_file_path;
            std::string m_element_container_storage_file_path;
            // Locks
            std::mutex m_element_container_lock;

        // Getter Methods
        public:
            std::string GetElementContainerName() const;
            int GetElementContainerSize() const;
            std::string GetElementContainerPath() const;
            std::string GetElementContainerDataFilePath() const;
            std::string GetElementContainerStorageFilePath() const;

        // Utilities Methods
        public:
            std::string GetElementContainerData(int);
            void UpdateElementContainerSize(int);
            void LockExclusiveElementOperations();
            void UnlockExclusiveElementOperations();

    };

} // namespace pandora

#endif