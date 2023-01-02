// *************************************
// Pandora Storage Server
// Storage Core
// 'shard.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SHARD_H
#define SHARD_H

#include <string>

namespace pandora {

    class Shard {

        // Constructor
        public:
            Shard(const std::string&, const std::string&, const bool&);

        // Properties
        private:
            // General properties
            std::string m_shard_name;
            int m_shard_size;
            // Paths
            std::string m_shard_path;
            std::string m_shard_data_file_path;
            std::string m_shard_storage_file_path;

        // Getter Methods
        public:
            std::string GetShardName() const;
            int GetShardSize() const;
            std::string GetShardPath() const;
            std::string GetShardDataFilePath() const;
            std::string GetShardStorageFilePath() const;

        // Utilities Methods
        public:
            std::string GetShardData(int);
            void UpdateShardSize(int);

    };

} // namespace pandora

#endif