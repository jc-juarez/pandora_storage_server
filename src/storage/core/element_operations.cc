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
#include <memory>
#include <future>
#include <vector>
#include <string>
#include <atomic>
#include <array>

namespace pandora {

    namespace core {

        bool MatchesDelimeter(int start, const std::string& element) {

            // Analyze matching delimeter in Element
            for(size_t element_index = start, delimeter_index = 0; delimeter_index < pandora::constants::element_delimeter.size(); ++element_index, ++delimeter_index) {
                if(pandora::constants::element_delimeter[delimeter_index] != element[element_index]) return false;
            }

            return true;

        }

        bool FindID(const std::string& element_id, char* element) {

            // Analyze matching ID in Element
            for(size_t i = 0; i < element_id.size(); ++i) {
                if(element_id[i] != element[i]) return false;
            }

            return true;

        }

        bool ElementIsValid(const std::string& element_id, const std::string& element) {

            int remaining_characters = element.size() - element_id.size();

            // Analyze well-formed Element
            if(remaining_characters > pandora::constants::element_delimeter.size())
                if(MatchesDelimeter(element_id.size(), element)) return true;

            return false;

        }

        bool MatchesID(const std::string& element_id, char* element) {

            // Analyze if Element ID matches to the current Element
            if(FindID(element_id, element))
                if(ElementIsValid(element_id, element)) return true;
            
            return false;

        }

        std::string GetElementLineOrValue(pandora::constants::LineValueOption line_value_option, const std::string& element_id, const std::string& element_container_storage_path, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data, std::shared_ptr<std::atomic_bool>& stop_search_threads_flag) {

            // Reference Element Container Storage File
            std::ifstream element_container_storage_file(element_container_storage_path, std::ios::in);

            int line_number {};

            // Element holder
            const size_t max_element_size {pandora::constants::ElementIDMaxSize + pandora::constants::element_delimeter.size() + pandora::constants::ElementValueMaxSize};
            char* element = new char[max_element_size];

            try {

                while(element_container_storage_file.getline(element, max_element_size)) {
                    if(stop_search_threads_flag->load()) pandora::constants::not_found_int;
                    ++line_number;
                    // Look for matching Element ID
                    if(MatchesID(element_id, element)) {

                        stop_search_threads_flag->store(true);

                        std::string result {};

                        // Determine result to return
                        if(line_value_option == pandora::constants::LineValueOption::Line) {
                            result = std::to_string(line_number);
                        } else {
                            int start_index = element_id.size() + pandora::constants::element_delimeter.size();
                            std::string element_cpp_string {element};
                            result = element_cpp_string.substr(start_index);
                        }

                        delete[] element;
                        return result;

                    }
                }

                delete[] element;
                return pandora::constants::not_found_string;

            } catch(...) {

                // Error catching on Element Container traversal
                delete[] element;
                request_data.log.append("An Error ocurred while trying to find an Element from the required Element Container.");
                server_options->LogError(pandora::constants::ElementStorageError, request_data);

            }

            return pandora::constants::not_found_string;
        }

        std::pair<std::string, int> SearchElementInterface(pandora::constants::LineValueOption line_value_option, std::vector<std::string> shard_segment, const std::string& element_id, pandora::ServerOptions* server_options, pandora::utilities::RequestData& request_data, std::shared_ptr<std::atomic_bool>& stop_search_threads_flag) {

            for(int shard_subindex = 0; shard_subindex < shard_segment.size(); ++shard_subindex) {
                std::string result = GetElementLineOrValue(line_value_option, element_id, shard_segment[shard_subindex], server_options, request_data, stop_search_threads_flag);
                if(result != pandora::constants::not_found_string) return {result, shard_subindex};
            }

            return {pandora::constants::not_found_string, pandora::constants::not_found_int};

        }

        std::pair<std::string, int> WaitForSearchResult(pandora::constants::LineValueOption line_value_option, std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::RequestData& request_data, ElementContainer& element_container, const std::string& element_id, int shard_segment_size) {

            // Initialize Search Threads flag 
            std::shared_ptr<std::atomic_bool> stop_search_threads_flag = std::make_shared<std::atomic_bool>(false);

            // Search Threads container that maps Element Line to Shard Index
            std::array<std::future<std::pair<std::string, int>>, pandora::constants::number_search_threads> search_threads {};

            // Launch all Search Threads
            for(int segment_thread_index = 0; segment_thread_index < pandora::constants::number_search_threads; ++segment_thread_index) {
                
                std::vector<std::string> shard_segment {};

                for(int shard_subindex = 0; shard_subindex < element_container.GetShardSegmentSize(); ++shard_subindex) {

                    shard_segment.push_back(element_container.GetShard((segment_thread_index * element_container.GetShardSegmentSize()) + shard_subindex).GetShardStorageFilePath());

                }

                search_threads[segment_thread_index] = std::async(std::launch::async, SearchElementInterface, line_value_option, shard_segment,
                                                                  std::ref(element_id), main_data->GetServerOptions(), std::ref(request_data), std::ref(stop_search_threads_flag));

            }

            std::unordered_set<int> finished_search_threads {};

            while(finished_search_threads.size() < pandora::constants::number_search_threads) {

                for(int segment_thread_index = 0; segment_thread_index < pandora::constants::number_search_threads && finished_search_threads.size() < pandora::constants::number_search_threads; ++segment_thread_index) {

                    if(finished_search_threads.count(segment_thread_index)) continue;
                    if(!(search_threads[segment_thread_index].wait_for(std::chrono::seconds(0)) == std::future_status::ready)) continue;

                    finished_search_threads.insert(segment_thread_index);
                    std::pair<std::string, int> result = search_threads[segment_thread_index].get();

                    if(result.first != pandora::constants::not_found_string) return {result.first, (segment_thread_index * shard_segment_size) + result.second};

                }

            }

            // Element not found in any Shard
            return {pandora::constants::not_found_string, pandora::constants::not_found_int};

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

            // Launch Search Threads and Wait for search result (pair maps Element Line to Shard Index)
            std::pair<std::string, int> search_result = WaitForSearchResult(pandora::constants::LineValueOption::Line, main_data, 
                                                                            request_data, element_container, element_id, element_container.GetShardSegmentSize());

            // Remove previous element if already exists
            bool element_exists = search_result.first != pandora::constants::not_found_string ? true : false; 
            if(element_exists) {
                // Remove Element
                pandora::storage::RemoveOrReplaceFileLine(pandora::constants::RemoveOrReplace::Remove, std::stoi(search_result.first), element_container.GetShard(search_result.second).GetShardStorageFilePath());
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
            pandora::storage::FileOperation(element_container.GetShard(element_container.GetRoundRobinIndex()).GetShardStorageFilePath(), pandora::constants::FileOption::Append, element);

            // Update Shard size
            int current_shard_size = element_container.GetShard(element_container.GetRoundRobinIndex()).GetShardSize();
            element_container.GetShard(element_container.GetRoundRobinIndex()).UpdateShardSize(current_shard_size + 1);

            // Update Element Container size
            if(!element_exists) element_container.UpdateElementContainerSize(element_container.GetElementContainerSize() + 1);

            // Increase Element Container Round Robin Index
            element_container.IncreaseRoundRobinIndex();

            // Log Element succesful element set
            request_data.log.append("Element '" + element_id + "' has been set inside the Element Container '" +
                                     element_container_name + "'.");
            main_data->GetServerOptions()->LogInfo(request_data);

            // Unlock Exclusive Operation
            element_container.UnlockExclusiveElementOperations();

            // Unlock Shared operation
            main_data->UnlockSharedElementContainerOperations();

            // Set Live Memory
            
        }

        void DeleteElement(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::RequestData& request_data) {
            
            // Delete Element on Disk
            // Lock Shared operation
            main_data->LockSharedElementContainerOperations();

            // Element Container name
            std::string element_container_name {request_data.arguments[pandora::constants::element_container_name]};
            // Element ID
            std::string element_id {request_data.arguments[pandora::constants::element_id]};

            // Check if Element Container does not exist 
            if(!main_data->ElementContainerExists(element_container_name)) {
                request_data.log.append("Element Container '" + element_container_name + "' does not exist and Element '" +
                                         element_id + "' could not be deleted.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Element Container instance
            ElementContainer& element_container = main_data->GetElementContainer(element_container_name);

            // Lock Exclusive Operation
            element_container.LockExclusiveElementOperations();

            // Launch Search Threads and Wait for search result (pair maps Element Line to Shard Index)
            std::pair<std::string, int> search_result = WaitForSearchResult(pandora::constants::LineValueOption::Line, main_data, 
                                                                            request_data, element_container, element_id, element_container.GetShardSegmentSize());

            // Determine if the Element exists
            bool element_exists = search_result.first != pandora::constants::not_found_string ? true : false; 

            // Check if Element exists
            if(!element_exists) {
                request_data.log.append("Element Container '" + element_container_name + "' does not contain Element '" +
                                         element_id + "'.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementNotExists, request_data);
            }

            // Remove Element
            pandora::storage::RemoveOrReplaceFileLine(pandora::constants::RemoveOrReplace::Remove, std::stoi(search_result.first), element_container.GetShard(search_result.second).GetShardStorageFilePath());
            // Decrease Shard size
            int shard_size = element_container.GetShard(search_result.second).GetShardSize();
            element_container.GetShard(search_result.second).UpdateShardSize(shard_size - 1);
            
            // Update Element Container size
            element_container.UpdateElementContainerSize(element_container.GetElementContainerSize() - 1);

            // Log Element succesful element removal
            request_data.log.append("Element '" + element_id + "' has been deleted from the Element Container '" +
                                     element_container_name + "'.");
            main_data->GetServerOptions()->LogInfo(request_data);

            // Unlock Exclusive Operation
            element_container.UnlockExclusiveElementOperations();

            // Unlock Shared operation
            main_data->UnlockSharedElementContainerOperations();

            // Set Live Memory
            
        }

        std::string GetElement(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::RequestData& request_data) {

            // Retrieve from Live Memory

            // Retrieve from Disk
            // Lock shared operation
            main_data->LockSharedElementContainerOperations();

            // Element Container name
            std::string element_container_name {request_data.arguments[pandora::constants::element_container_name]};
            // Element ID
            std::string element_id {request_data.arguments[pandora::constants::element_id]};

            // Check if Element Container does not exist 
            if(!main_data->ElementContainerExists(element_container_name)) {
                request_data.log.append("Element Container '" + element_container_name + "' does not exist and Element '" +
                                         element_id + "' could not be retrieved.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerNotExistsErrorCode, request_data);
            }

            // Element Container instance
            ElementContainer& element_container = main_data->GetElementContainer(element_container_name);

            // Lock shared operation
            element_container.LockSharedElementOperations();

            // Launch Search Threads and Wait for search result (pair maps Element Value to Shard Index)
            std::pair<std::string, int> search_result = WaitForSearchResult(pandora::constants::LineValueOption::Value, main_data, 
                                                                            request_data, element_container, element_id, element_container.GetShardSegmentSize());

            // Assign Element Value
            std::string element_value = search_result.first;

            // Check if Element exists
            if(element_value == pandora::constants::not_found_string) {
                request_data.log.append("Element Container '" + element_container_name + "' does not contain Element '" +
                                         element_id + "'.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementNotExists, request_data);
            }

            // Log succesful element retrieval
            request_data.log.append("Element '" + element_id + "' has value: '" + element_value + "'.");
            main_data->GetServerOptions()->LogInfo(request_data);

            // Unlock shared operations
            element_container.UnlockSharedElementOperations();
            main_data->UnlockSharedElementContainerOperations();

            return element_value;

        }

    }

} // namespace pandora