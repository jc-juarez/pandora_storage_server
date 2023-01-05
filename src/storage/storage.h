// *************************************
// Pandora Storage Server
// Storage
// 'storage.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef STORAGE_OPERATIONS_H
#define STORAGE_OPERATIONS_H

#include "../server/server_constants.h"
#include <filesystem>
#include <string>

namespace pandora {

    namespace storage {

        void CreateDirectory(const std::filesystem::path);
        void FileOperation(const std::string, pandora::constants::FileOption, const std::string = "");
        void RemoveOrReplaceFileLine(pandora::constants::RemoveOrReplace, int, const std::string&, const std::string = "");

    }

} // namespace pandora

#endif