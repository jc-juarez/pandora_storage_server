// *************************************
// Pandora Storage Server
// Live Memory System
// 'element_container_cache.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_CONTAINER_CACHE_H
#define ELEMENT_CONTAINER_CACHE_H

#include <unordered_map>
#include <shared_mutex>
#include <string>

namespace pandora {

    class ElementContainerCache {

        // Constructor
        public:
            ElementContainerCache();

        // Mutex Locks
        private:
            std::unordered_map<std::string, std::shared_mutex> element_container_locks;
            std::shared_mutex m_delete_element_container_mutex;

        // Mutex Operations
        public:
            // Delete Element Container
            void LockSharedDeleteElementContainerOperation();
            void UnlockSharedDeleteElementContainerOperation();
            void LockExclusiveDeleteElementContainerOperation();
            void UnlockExclusiveDeleteElementContainerOperation();

        // Cache System
        public:
            void InitialLiveMemoryFilling();

    };

}

#endif