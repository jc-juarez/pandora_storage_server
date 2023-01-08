// *************************************
// Pandora Storage Server
// Storage Core
// 'shard.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_constants.h"
#include "../storage.h"
#include "shard.h"
#include <fstream>
#include <string>

namespace pandora {

    Shard::Shard(const std::string& element_container_shards_path, const std::string& shard_name, const bool& element_container_exists) {

        m_shard_name.append(shard_name);
        m_shard_size = 0;

        m_shard_path.append(element_container_shards_path + "/" + GetShardName());
        m_shard_data_file_path.append(GetShardPath() + "/" + pandora::constants::data);
        m_shard_storage_file_path.append(GetShardPath() + "/" + pandora::constants::storage);

        if(!element_container_exists) {

            // Create Shard
            pandora::storage::CreateDirectory(GetShardPath());
            // Data File creation
            pandora::storage::FileOperation(GetShardDataFilePath(), pandora::constants::FileOption::Create);
            // Storage File creation
            pandora::storage::FileOperation(GetShardStorageFilePath(), pandora::constants::FileOption::Create);

        }
        // Element Container already exists on Disk and is being brought to Memory on server startup 
        else {

            try {

                // Recover Shard size
                // If Data File is healthy recover from Disk
                if(pandora::storage::CountFileLines(GetShardDataFilePath()) == pandora::constants::shard_data_file_size) {
                    m_shard_size = std::stoi(GetShardData(pandora::constants::shard_datafile_size_index));
                    return;
                } 

            } catch(...) {

                // Data File is corrupted; hard recover 
                // Data File creation
                pandora::storage::FileOperation(GetShardDataFilePath(), pandora::constants::FileOption::Create);

                for(int shard_index = 0; shard_index < pandora::constants::number_shards; ++shard_index) {
                    m_shard_size = pandora::storage::CountFileLines(GetShardStorageFilePath());
                }

            }

        }

        // Fill Data File
        pandora::storage::FileOperation(GetShardDataFilePath(), pandora::constants::FileOption::Append, std::to_string(GetShardSize()) + "\n");
        pandora::storage::FileOperation(GetShardDataFilePath(), pandora::constants::FileOption::Append, GetShardPath() + "\n");

    }

    std::string Shard::GetShardName() const { return m_shard_name; }

    int Shard::GetShardSize() const {return m_shard_size; }

    std::string Shard::GetShardPath() const { return m_shard_path; }

    std::string Shard::GetShardDataFilePath() const { return m_shard_data_file_path; }

    std::string Shard::GetShardStorageFilePath() const { return m_shard_storage_file_path; }

    std::string Shard::GetShardData(int data_index) {

        std::ifstream shard_data_file;
        shard_data_file.open(GetShardDataFilePath());

        std::string line {};
        int current_index {0};

        while (std::getline(shard_data_file, line)) {
            if(current_index == data_index) {
                return line;
            }
            ++current_index;
        }

        return pandora::constants::not_found_string;

    }

    void Shard::UpdateShardSize(int size) {

        pandora::storage::RemoveOrReplaceFileLine(pandora::constants::RemoveOrReplace:: Replace, pandora::constants::shard_datafile_size_index + 1, GetShardDataFilePath(), std::to_string(size));
        m_shard_size = size;

    }

} // namespace pandora