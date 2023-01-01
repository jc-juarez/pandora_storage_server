// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container_operations.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_CONTAINER_OPERATIONS_H
#define ELEMENT_CONTAINER_OPERATIONS_H

#include "../../server/server_utilities.h"
#include "live-memory/element_container_cache.h"
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    class MainData;

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::MainData>&, pandora::utilities::RequestData&);
        void DeleteElementContainer(std::shared_ptr<pandora::MainData>&, pandora::utilities::RequestData&);

    }

} // namespace pandora

#endif