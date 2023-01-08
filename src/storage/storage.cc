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

        void FileOperation(const std::string file_path, pandora::constants::FileOption file_option, const std::string content) {

            std::ofstream outfile;

            if(file_option == pandora::constants::FileOption::Create) {
                outfile.open(file_path);
                return;
            }

            outfile.open(file_path, std::ios_base::app);

            outfile << content;

        }

        void RemoveOrReplaceFileLine(pandora::constants::RemoveOrReplace remove_or_replace, int line_number, const std::string& file_path, const std::string replacement) {
            
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
            temp_file_name.append(pandora::utilities::GetRandomString_Size9() + ".txt");
            write_file.open(temp_file_name);

            for(size_t i = 0; i < lines.size(); ++i) {
                if(i == line_number) {
                    if(remove_or_replace == pandora::constants::RemoveOrReplace::Remove) continue;
                    write_file << replacement << std::endl; 
                } else {
                    write_file << lines[i] << std::endl; 
                }
            }

            write_file.close();

            const char* file_path_cstring = file_path.c_str();
            const char* temp_file_name_cstring = temp_file_name.c_str();
            std::remove(file_path_cstring);
            std::rename(temp_file_name_cstring, file_path_cstring);

        }

        int CountFileLines(const std::string& file_path) {

            std::ifstream read_file;
            read_file.open(file_path);

            int number_lines {0};
            std::string line {};

            while(std::getline(read_file, line)) {
                if(!line.empty() && line[0] != '\0') ++number_lines;
            }

            return number_lines;

        }

    }

} // namespace pandora