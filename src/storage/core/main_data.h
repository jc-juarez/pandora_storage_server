// *************************************
// Pandora Storage Server
// Storage Core
// 'main_data.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include "live-memory/element_container_cache.h"
#include "element_container.h"
#include <unordered_map>
#include <shared_mutex>
#include <utility>
#include <memory>
#include <string>

namespace pandora {

    class ServerOptions;
    class ElementContainerCache;

    class MainData {

        // Constructor
        public:
            MainData(std::shared_ptr<ElementContainerCache>&, ServerOptions*);

        // No Copy Constructor
        public:
            MainData(const MainData&) = delete;

        // Properties
        private:
            // Object properties
            ServerOptions* m_server_options;
            std::shared_ptr<ElementContainerCache> m_main_cache;
            std::unordered_map<std::string, ElementContainer> m_element_containers;
            // General properties
            std::shared_mutex m_element_container_operations_mutex;

        public:
            // Getter Methods
            ServerOptions* GetServerOptions();
            std::shared_ptr<ElementContainerCache>& GetMainCache();
            ElementContainer& GetElementContainer(const std::string&);

        public:
            // Utilities Methods
            void AddElementContainer(const std::string&);
            void DeleteElementContainer(const std::string&);
            void LockExclusiveElementContainerOperations();
            void UnlockExclusiveElementContainerOperations();
            void LockSharedElementContainerOperations();
            void UnlockSharedElementContainerOperations();
            bool ElementContainerExists(const std::string&);

    };

} // namespace pandora

#endif