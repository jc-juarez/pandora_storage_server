// *************************************
// Pandora Storage Server
// Storage
// 'storage.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include <filesystem>
#include <fstream>
#include <ios>

namespace pandora {

    namespace storage {

        void CreateDirectory(const std::filesystem::path path) {
            try {
                std::filesystem::create_directories(path);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not create the " + std::string(path) + " directory.");
                throw std::runtime_error(error);
            }
        }

        void AddFileContent(const std::string file_path, const std::string content, bool append) {
            std::ofstream outfile;
            if (append)
                outfile.open(file_path, std::ios_base::app);
            else
                outfile.open(file_path);
            outfile << content;
        }

    }

} // namespace pandora