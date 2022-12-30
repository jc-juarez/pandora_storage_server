// *************************************
// Pandora Storage Server
// Storage
// 'storage.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../server/server_constants.h"
#include "../server/server_utilities.h"
#include <filesystem>
#include <fstream>
#include <vector>
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

        void ReplaceFileLine(int line_number, const std::string& file_path, const std::string replacement) {
            
            if(line_number == pandora::constants::not_found_int) return;

            std::ifstream read_file;
            read_file.open(file_path);

            std::vector<std::string> lines {};
            std::string line {};

            while (std::getline(read_file, line)) lines.push_back(line);

            read_file.close();

            line_number--;

            std::ofstream write_file;
            std::string temp_file_name {"tempfile-"};
            temp_file_name.append(pandora::utilities::GetRandomString_Size8() + ".txt");
            write_file.open(temp_file_name);

            for(size_t i = 0; i < lines.size(); ++i) {
                if(i == line_number) {
                    // If no replacement argument was given the line will be deleted
                    if(replacement.empty()) continue;
                    write_file << replacement << std::endl; 
                } else {
                    write_file << lines[i] << std::endl; 
                }
            }

            write_file.close();

            const char* element_container_storage_path_cstring = file_path.c_str();
            const char* temp_file_name_cstring = temp_file_name.c_str();
            std::remove(element_container_storage_path_cstring);
            std::rename(temp_file_name_cstring, element_container_storage_path_cstring);
        }

    }

} // namespace pandora