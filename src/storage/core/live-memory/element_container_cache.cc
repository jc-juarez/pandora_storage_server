// *************************************
// Pandora Storage Server
// Live Memory System
// 'element_container_cache.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "element_container_cache.h"

namespace pandora {

    ElementContainerCache::ElementContainerCache() {}

    void ElementContainerCache::LockSharedDeleteElementContainerOperation() { m_delete_element_container_mutex.lock_shared(); }

    void ElementContainerCache::UnlockSharedDeleteElementContainerOperation() { m_delete_element_container_mutex.unlock_shared(); }

    void ElementContainerCache::LockExclusiveDeleteElementContainerOperation() { m_delete_element_container_mutex.lock(); }

    void ElementContainerCache::UnlockExclusiveDeleteElementContainerOperation() { m_delete_element_container_mutex.unlock(); }

    void ElementContainerCache::InitialLiveMemoryFilling() {}

}