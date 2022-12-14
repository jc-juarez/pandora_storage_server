// *************************************
// Pandora Storage Server
// Live Memory System
// 'element_container_cache.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "element_container_cache.h"

namespace pandora {

    std::shared_mutex ElementContainerCache::delete_element_container_mutex;

    ElementContainerCache::ElementContainerCache() {}

    void ElementContainerCache::InitialLiveMemoryFilling() {}

}