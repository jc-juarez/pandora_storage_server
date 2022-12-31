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

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);
        void DeleteElementContainer(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);
        std::string GetElementContainerData(int, const std::string&);

    }

} // namespace pandora

#endif