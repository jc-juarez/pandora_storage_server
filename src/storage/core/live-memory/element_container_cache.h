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
#include <string>

namespace pandora {

    class ElementContainerCache {

        // Constructor
        public:
            ElementContainerCache();

        // Cache System
        public:
            void InitialLiveMemoryFilling();

    };

}

#endif