// *************************************
// Pandora Storage Server
// Server
// 'orion_engine.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ORION_ENGINE_H
#define ORION_ENGINE_H

#include <shared_mutex>

namespace pandora {

    namespace core {

        // Storage Synchronization
        extern std::shared_mutex delete_elements_container_mutex;

    }

} // namespace pandora

#endif