// *************************************
// Pandora Storage Server
// Storage
// 'storage_operations.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef STORAGE_OPERATIONS_H
#define STORAGE_OPERATIONS_H

#include <filesystem>
#include <string>

namespace pandora {

    namespace storage_operations {

        void AddFileContent(const std::string, const std::string, bool);

    }

} // namespace pandora

#endif