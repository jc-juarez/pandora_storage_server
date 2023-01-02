// *************************************
// Pandora Storage Server
// Storage Core
// 'element_operations.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "element_container.h"
#include "main_data.h"
#include "live-memory/element_container_cache.h"
#include "element_container.h"
#include <unordered_set>
#include <filesystem>
#include <iostream>
#include <utility>
#include <fstream>
#include <future>
#include <vector>
#include <string>
#include <array>

namespace pandora {

    namespace core {

        bool MatchesDelimeter(int start, const std::string& element) {

            for(size_t element_index = start, delimeter_index = 0; delimeter_index < pandora::constants::element_delimeter.size(); ++element_index, ++delimeter_index) {
                if(pandora::constants::element_delimeter[delimeter_index] != element[element_index]) return false;
            }

            return true;
        }

        bool FindID(const std::string& element_id, char* element) {

            // Look for matching ID in current Element
            for(size_t i = 0; i < element_id.size(); ++i) {
                if(element_id[i] != element[i]) return false;
            }

            return true;
        }

        bool ElementIsValid(const std::string& element_id, const std::string& element) {

            int remaining_characters = element.size() - element_id.size();

            if(remaining_characters > pandora::constants::element_delimeter.size())
                if(MatchesDelimeter(element_id.size(), element)) return true;

            return false;
        }

        bool MatchesID(const std::string& element_id, char* element) {

            if(FindID(element_id, element))
                if(ElementIsValid(element_id, element)) return true;
            
            return false;
        }

        int GetElementLineNumber(const std::string& element_id, const std::string& element_container_storage_path, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data, std::atomic_bool* stop_search_threads) {

            // Reference Element Container Storage File
            std::ifstream element_container_storage_file(element_container_storage_path, std::ios::in);

            int line_number {};

            // Element holder
            const size_t max_element_size {pandora::constants::ElementIDMaxSize + pandora::constants::element_delimeter.size() + pandora::constants::ElementValueMaxSize};
            char* element = new char[max_element_size];

            try {

                while(element_container_storage_file.getline(element, max_element_size)) {
                    if(stop_search_threads->load()) pandora::constants::not_found_int;
                    ++line_number;
                    // Look for matching Element ID and return line number
                    if(MatchesID(element_id, element)) {
                        delete[] element;
                        stop_search_threads->store(true);
                        return line_number;
                    }
                }

                delete[] element;
                return pandora::constants::not_found_int;

            } catch(...) {

                // Error catching on Element Container traversal
                delete[] element;
                request_data.log.append("An Error ocurred while trying to find an Element from the required Element Container.");
                server_options->LogError(pandora::constants::ElementStorageError, request_data);

            }

            return pandora::constants::not_found_int;
        }

        std::pair<int, int> WaitForSearchResult(std::array<std::future<std::pair<int, int>>, pandora::constants::number_search_threads>& search_threads, int shard_segments_size) {

            std::unordered_set<int> finished_search_threads {};

            while(finished_search_threads.size() < pandora::constants::number_search_threads) {

                for(int segment_thread_index = 0; segment_thread_index < pandora::constants::number_search_threads && finished_search_threads.size() < pandora::constants::number_search_threads; ++segment_thread_index) {

                    if(finished_search_threads.count(segment_thread_index)) continue;
                    if(!(search_threads[segment_thread_index].wait_for(std::chrono::seconds(0)) == std::future_status::ready)) continue;

                    finished_search_threads.insert(segment_thread_index);
                    std::pair<int, int> result = search_threads[segment_thread_index].get();

                    if(result.first != pandora::constants::not_found_int) return {result.first, (segment_thread_index * shard_segments_size) + result.second };

                }

            }

            // Element not found in any Shard
            return {pandora::constants::not_found_int, pandora::constants::not_found_int};

        }

        std::pair<int, int> SearchElementInterface(std::vector<std::string> shard_segment, const std::string& element_id, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data, std::atomic_bool* stop_search_threads) {

            for(int shard_subindex = 0; shard_subindex < shard_segment.size(); ++shard_subindex) {
                int element_line = GetElementLineNumber(element_id, shard_segment[shard_subindex], server_options, request_data, stop_search_threads);
                if(element_line != pandora::constants::not_found_int) return {element_line, shard_subindex};
            }

            return {pandora::constants::not_found_int, pandora::constants::not_found_int};

        }

        void SetElement(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::RequestData& request_data) {
            
            // Set Element on Disk
            // Lock Shared operation
            main_data->LockSharedElementContainerOperations();

            // Element Container name
            std::string element_container_name {request_data.arguments[pandora::constants::element_container_name]};
            // Element ID
            std::string element_id {request_data.arguments[pandora::constants::element_id]};
            // Element Value
            std::string element_value {request_data.arguments[pandora::constants::element_value]};

            // Check if Element Container does not exist 
            if(!main_data->ElementContainerExists(element_container_name)) {
                request_data.log.append("Element Container '" + element_container_name + "' does not exist and Element '" +
                                         element_id + "' could not be set.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Element Container instance
            ElementContainer& element_container = main_data->GetElementContainer(element_container_name);

            // Lock Exclusive Operation
            element_container.LockExclusiveElementOperations();

            // Check if Element Container exceeds capacity
            if(element_container.GetElementContainerSize() == pandora::constants::ElementContainerMaxCapacity) {
                request_data.log.append("Element Container '" + element_container.GetElementContainerName() + "' has reached the max number of Elements (" + 
                                         std::to_string(pandora::constants::ElementContainerMaxCapacity) + ") and cannot add more Elements.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerFull, request_data);
            }

            // Initialize search threads 
            element_container.m_stop_search_threads->store(false);

            // Search Threads container that maps Element Line to Shard Index
            std::array<std::future<std::pair<int, int>>, pandora::constants::number_search_threads> search_threads {};

            // Launch all Search Threads
            for(int segment_thread_index = 0; segment_thread_index < pandora::constants::number_search_threads; ++segment_thread_index) {
                
                std::vector<std::string> shard_segment {};

                for(int shard_subindex = 0; shard_subindex < element_container.GetShardSegmentsSize(); ++shard_subindex) {

                    shard_segment.push_back(element_container.GetShard((segment_thread_index * element_container.GetShardSegmentsSize()) + shard_subindex).GetShardStorageFilePath());

                }

                search_threads[segment_thread_index] = std::async(std::launch::async, SearchElementInterface, shard_segment,
                                                                  std::ref(element_id), main_data->GetServerOptions(), std::ref(request_data), element_container.m_stop_search_threads);

            }

            // Wait for search result (pair maps Element Line to Shard Index)
            std::pair<int, int> search_result = WaitForSearchResult(search_threads, element_container.GetShardSegmentsSize());

            // Initialize search threads 
            element_container.m_stop_search_threads->store(false);

            // Remove previous element if already exists
            bool element_exists = search_result.first != pandora::constants::not_found_int ? true : false; 
            if(element_exists) {
                // Remove Element
                pandora::storage::ReplaceFileLine(search_result.first, element_container.GetShard(search_result.second).GetShardStorageFilePath());
                // Decrease Shard size
                int shard_size = element_container.GetShard(search_result.second).GetShardSize();
                element_container.GetShard(search_result.second).UpdateShardSize(shard_size - 1);
            }
            
            // Construct Element
            std::string element {};

            // Append Element ID
            element.append(element_id);
            // Append Delimeter
            element.append(pandora::constants::element_delimeter);
            // Append Value
            element.append(element_value + "\n");

            // Look for non-full Shard to insert Element
            int full_shards_count {};

            while(element_container.GetShard(element_container.GetRoundRobinIndex()).GetShardSize() == pandora::constants::ShardMaxCapacity) {

                if(full_shards_count == pandora::constants::number_shards) {
                    request_data.log.append("Element Container '" + element_container.GetElementContainerName() + "' has reached the max number of Elements (" + 
                                             std::to_string(pandora::constants::ElementContainerMaxCapacity) + ") and cannot add more Elements.");
                    main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerFull, request_data);
                }

                ++full_shards_count;
                element_container.IncreaseRoundRobinIndex();

            }

            // Add Element to Shard
            pandora::storage::AddFileContent(element_container.GetShard(element_container.GetRoundRobinIndex()).GetShardStorageFilePath(), element, true);

            // Update Shard size
            int current_shard_size = element_container.GetShard(element_container.GetRoundRobinIndex()).GetShardSize();
            element_container.GetShard(element_container.GetRoundRobinIndex()).UpdateShardSize(current_shard_size + 1);

            // Update Element Container size
            if(!element_exists) element_container.UpdateElementContainerSize(element_container.GetElementContainerSize() + 1);

            // Increase Element Container Round Robin Index
            element_container.IncreaseRoundRobinIndex();

            // Log Element succesful set
            request_data.log.append("Element '" + element_id + "' has been set inside the Element Container '" +
                                     element_container_name + "'.");
            main_data->GetServerOptions()->LogInfo(request_data);

            // Unlock Exclusive Operation
            element_container.UnlockExclusiveElementOperations();

            // Unlock Shared operation
            main_data->UnlockSharedElementContainerOperations();

            // Set Live Memory
            
        }

        std::string GetElement(std::shared_ptr<pandora::ElementContainerCache>& main_cache, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data) {

            // Retrieve from Live Memory

            // Retrieve from Disk
            // Lock shared operation
            main_cache->LockSharedDeleteElementContainerOperation();
            
            // Element Container Path
            std::string element_container_path {pandora::constants::element_containers_directory_path};
            element_container_path.append("/" + request_data.arguments[pandora::constants::element_container_name]);

            // Check if Element Container does not exist 
            if(!std::filesystem::exists(element_container_path)) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not exist and Element '" +
                                         request_data.arguments[pandora::constants::element_id] + "' could not be retrieved.");
                server_options->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Element Container Storage File path
            std::string element_container_storage_path {element_container_path};
            element_container_storage_path.append("/" + pandora::constants::storage);
            std::ifstream element_container_storage_file;
            element_container_storage_file.open(element_container_storage_path);

            std::string element_value {};

            // Element holder
            const size_t max_element_size {pandora::constants::ElementIDMaxSize + pandora::constants::element_delimeter.size() + pandora::constants::ElementValueMaxSize};
            char* element = new char[max_element_size];

            try {

                while(element_container_storage_file.getline(element, max_element_size)) {
                    if(MatchesID(request_data.arguments[pandora::constants::element_id], element)) {
                        int start_index = request_data.arguments[pandora::constants::element_id].size() + pandora::constants::element_delimeter.size();
                        std::string element_cpp_string {element};
                        element_value = element_cpp_string.substr(start_index);
                        break;
                    }
                }

                delete[] element;

            } catch(...) {

                // Error catching on Element Container traversal
                delete[] element;
                request_data.log.append("An Error ocurred while trying to find an Element from the required Element Container.");
                server_options->LogError(pandora::constants::ElementStorageError, request_data);

            }

            if(element_value.empty()) {
                request_data.log.append("Element Container '" + request_data.arguments[pandora::constants::element_container_name] + "' does not contain Element '" +
                                         request_data.arguments[pandora::constants::element_id] + "'.");
                server_options->LogError(pandora::constants::ElementNotExists, request_data);
            }

            request_data.log.append("Element '" + request_data.arguments[pandora::constants::element_id] + "' has value: '" + element_value + "'.");
            server_options->LogInfo(request_data);

            // Unlock shared operation
            main_cache->UnlockSharedDeleteElementContainerOperation();

            return element_value;

        }

    }

} // namespace pandora