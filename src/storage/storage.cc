// *************************************
// Pandora Storage Server
// Storage
// 'storage.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include <fstream>
#include <ios>

namespace pandora {

    namespace storage {

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