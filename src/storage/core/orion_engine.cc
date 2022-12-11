// *************************************
// Pandora Storage Server
// Server
// 'orion_engine.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include <shared_mutex>

namespace pandora {

    namespace core {

        // Storage Synchronization
        std::shared_mutex delete_elements_container_mutex;

        void InitialLiveMemoryFilling() {

        }

    }

} // namespace pandora