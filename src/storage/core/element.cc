// *************************************
// Pandora Storage Server
// Storage Core
// 'element.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "live-memory/element_container_cache.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace pandora {

    namespace core {

        bool MatchID(const std::string& element_id, char* line) {
            for(size_t i = 0; i < element_id.size(); ++i) {
                if(element_id[i] != line[i]) return false;
            }
            return true;
        }

        int GetElementLine(const std::string& element_id, const std::string& element_container_path) {

            std::ifstream element_container_file(element_container_path, std::ios::in);

            int line_number {};

            const int max_element_size {pandora::constants::ElementIDMaxSize + pandora::constants::element_delimeter.size() + pandora::constants::ElementValueMaxSize};
            char* line = new char[max_element_size];
            
            while(element_container_file.getline(line, max_element_size)) {
                ++line_number;
                if(MatchID(element_id, line)) {
                    delete[] line;
                    return line_number;
                }
            }

            delete[] line;
            return -1;
        }

        void RemoveElement(int line_number, const std::string& element_container_path) {

            std::ifstream element_container_file;
            element_container_file.open(element_container_path);

            std::vector<std::string> lines {};
            std::string line {};

            while (std::getline(element_container_file, line)) lines.push_back(line);

            element_container_file.close();

            line_number--;

            std::ofstream out_file;
            std::string temp_file_name {"tempfile-"};
            temp_file_name.append(pandora::utilities::GetRandomString_Size8() + ".txt");
            out_file.open(temp_file_name);

            for(int i = 0; i < lines.size(); ++i) {
                if(i != line_number) 
                    out_file << lines[i] << std::endl; 
            }

            out_file.close();

            const char* element_container_path_cstring = element_container_path.c_str();
            const char* temp_file_name_cstring = temp_file_name.c_str();
            std::remove(element_container_path_cstring);
            std::rename(temp_file_name_cstring, element_container_path_cstring);
        }

        void SetElement(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {

            // Add File for fast data status, add all mutexes unlocked at thread catch
            //* if(element_container_size == max_size) LogError

            // Set Element on Disk
            pandora::ElementContainerCache::delete_element_container_mutex.lock_shared();

            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            if(!std::filesystem::exists(element_container_path)) {
                pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not exist and Element '" +
                                         request_data.arguments[pandora::constants::element_id] + "' could not be set.");
                server_options->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Remove previous element if already exists
            RemoveElement(GetElementLine(request_data.arguments[pandora::constants::element_id], element_container_path),
                                         element_container_path);
            
            std::string element {};

            // Append Element ID
            element.append(request_data.arguments[pandora::constants::element_id]);
            // Append Delimeter
            element.append(pandora::constants::element_delimeter);
            // Append Value
            element.append(request_data.arguments[pandora::constants::element_value] + "\n");

            pandora::storage::AddFileContent(element_container_path, element, true);

            request_data.log.append("Element '" + request_data.arguments[pandora::constants::element_id] + "' has been set inside the Element Container '" +
                                     request_data.arguments[pandora::constants::element_container_name] + "'.");
            server_options->LogInfo(request_data);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();

            // Set Live Memory

        }

    }

} // namespace pandora