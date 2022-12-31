// *************************************
// Pandora Storage Server
// Storage Core
// 'element.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_H
#define ELEMENT_H

#include "../../server/server_utilities.h"
#include "live-memory/element_container_cache.h"
#include <string>

namespace pandora {

    namespace core {

        void SetElement(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);
        void DeleteElement(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);
        std::string GetElement(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*, pandora::utilities::RequestData&);

    }

} // namespace pandora

#endif