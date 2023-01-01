// *************************************
// Pandora Storage Server
// Storage
// 'storage.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef STORAGE_OPERATIONS_H
#define STORAGE_OPERATIONS_H

#include <filesystem>
#include <string>

namespace pandora {

    namespace storage {

        void CreateDirectory(const std::filesystem::path);
        void AddFileContent(const std::string, const std::string = "", bool = "false");
        void ReplaceFileLine(int, const std::string&, const std::string = "");

    }

} // namespace pandora

#endif