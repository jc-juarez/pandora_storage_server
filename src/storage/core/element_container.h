// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENTS_CONTAINER_H
#define ELEMENTS_CONTAINER_H

#include <sstream>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(const std::string&, const std::string&, std::stringstream&);

    }

} // namespace pandora

#endif