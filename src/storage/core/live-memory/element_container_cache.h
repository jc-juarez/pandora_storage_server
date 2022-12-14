// *************************************
// Pandora Storage Server
// Live Memory System
// 'element_container_cache.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_CONTAINER_CACHE_H
#define ELEMENT_CONTAINER_CACHE_H

#include <shared_mutex>

namespace pandora {

    class ElementContainerCache {

        // Static members
        public:
            static std::shared_mutex delete_element_container_mutex;

        // Constructor
        public:
            ElementContainerCache();

        // Cache System
        public:
            void InitialLiveMemoryFilling();

    };

}

#endif