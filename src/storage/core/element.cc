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

        std::string GetElementContainerData(int index, const std::string& element_container_data_path) {

            std::ifstream element_container_data_file;
            element_container_data_file.open(element_container_data_path);

            std::string line {};
            int element_container_current_size {};
            int current_index {1};

            while (std::getline(element_container_data_file, line)) {
                if(current_index == index) {
                    return line;
                }
                ++current_index;
            }

            return "";
        }

        bool MatchID(const std::string& element_id, char* line) {
            for(size_t i = 0; i < element_id.size(); ++i) {
                if(element_id[i] != line[i]) return false;
            }
            return true;
        }

        int GetElementLine(const std::string& element_id, const std::string& element_container_storage_path, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {

            std::ifstream element_container_storage_file(element_container_storage_path, std::ios::in);

            int line_number {};

            const size_t max_element_size {pandora::constants::ElementIDMaxSize + pandora::constants::element_delimeter.size() + pandora::constants::ElementValueMaxSize};
            char* line = new char[max_element_size];

            try {
                while(element_container_storage_file.getline(line, max_element_size)) {
                    ++line_number;
                    if(MatchID(element_id, line)) {
                        delete[] line;
                        return line_number;
                    }
                }
                delete[] line;
                return -1;
            } catch(...) {
                delete[] line;
                request_data.log.append("An Error ocurred while trying to find an Element from the required Element Container.");
                server_options->LogError(pandora::constants::ElementStorageError, request_data);
            }

            return -1;
        }

        void ReplaceLine(int line_number, const std::string& file_path, const std::string replacement = "") {
            
            if(line_number == -1) return;

            std::ifstream element_container_storage_file;
            element_container_storage_file.open(file_path);

            std::vector<std::string> lines {};
            std::string line {};

            while (std::getline(element_container_storage_file, line)) lines.push_back(line);

            element_container_storage_file.close();

            line_number--;

            std::ofstream out_file;
            std::string temp_file_name {"tempfile-"};
            temp_file_name.append(pandora::utilities::GetRandomString_Size8() + ".txt");
            out_file.open(temp_file_name);

            for(size_t i = 0; i < lines.size(); ++i) {
                if(i == line_number) {
                    // If no replacement argument was given the line will be deleted
                    if(replacement.empty()) continue;
                    out_file << replacement << std::endl; 
                } else {
                    out_file << lines[i] << std::endl; 
                }
            }

            out_file.close();

            const char* element_container_storage_path_cstring = file_path.c_str();
            const char* temp_file_name_cstring = temp_file_name.c_str();
            std::remove(element_container_storage_path_cstring);
            std::rename(temp_file_name_cstring, element_container_storage_path_cstring);
        }

        void SetElement(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {

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

            std::string element_container_data_path {element_container_path};
            element_container_data_path.append("/" + pandora::constants::data);
        
            int element_container_current_size {std::stoi(GetElementContainerData(pandora::constants::element_container_size_index, element_container_data_path))};

            if(element_container_current_size == pandora::constants::ElementContainerMaxSize) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' has reached the max number of Elements (" + 
                                         std::to_string(pandora::constants::ElementContainerMaxSize) + ") and cannot add more Elements.");
                server_options->LogError(pandora::constants::ElementContainerFull, request_data);
            }

            std::string element_container_storage_path {element_container_path};
            element_container_storage_path.append("/" + pandora::constants::storage);

            // Remove previous element if already exists
            int element_line_number = GetElementLine(request_data.arguments[pandora::constants::element_id], element_container_storage_path, server_options, request_data);
            bool element_exists = element_line_number != -1 ? true : false;
            
            if(element_exists) ReplaceLine(element_line_number, element_container_storage_path);

            std::string element {};

            // Append Element ID
            element.append(request_data.arguments[pandora::constants::element_id]);
            // Append Delimeter
            element.append(pandora::constants::element_delimeter);
            // Append Value
            element.append(request_data.arguments[pandora::constants::element_value] + "\n");

            pandora::storage::AddFileContent(element_container_storage_path, element, true);

            if(!element_exists) ReplaceLine(pandora::constants::element_container_size_index, element_container_data_path, std::to_string(element_container_current_size + 1));

            request_data.log.append("Element '" + request_data.arguments[pandora::constants::element_id] + "' has been set inside the Element Container '" +
                                     request_data.arguments[pandora::constants::element_container_name] + "'.");
            server_options->LogInfo(request_data);

            pandora::ElementContainerCache::delete_element_container_mutex.unlock_shared();

            // Set Live Memory

        }

    }

} // namespace pandora