// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENTS_CONTAINER_H
#define ELEMENTS_CONTAINER_H

#include "../../server/server_utilities.h"
#include "live-memory/element_container_cache.h"
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);

    }

} // namespace pandora

#endif