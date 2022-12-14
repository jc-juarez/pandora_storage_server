// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef ELEMENT_CONTAINER_H
#define ELEMENT_CONTAINER_H

#include "shard.h"
#include <shared_mutex>
#include <vector>
#include <string>

namespace pandora {

    class ElementContainer {

        // Constructor
        public:
            ElementContainer(const std::string&);

        // No Copy Constructor
        public:
            ElementContainer(const ElementContainer&) = delete;

        // Properties
        private:
            // General properties
            std::string m_element_container_name;
            int m_element_container_size;
            int m_round_robin_index;
            int m_shard_segments_size;
            std::vector<Shard> m_shards; 
            // Paths
            std::string m_element_container_path;
            std::string m_element_container_data_file_path;
            std::string m_element_container_shards_path;
            // Locks
            std::shared_mutex m_element_container_lock;

        // Getter Methods
        public:
            std::string GetElementContainerName() const;
            int GetElementContainerSize() const;
            int GetRoundRobinIndex() const;
            int GetShardSegmentSize() const;
            std::string GetElementContainerPath() const;
            std::string GetElementContainerDataFilePath() const;
            std::string GetElementContainerShardsPath() const;
            Shard& GetShard(const int&);

        // Utilities Methods
        public:
            void CreateShard(const int&, const bool&);
            std::string GetElementContainerData(int);
            void UpdateElementContainerSize(int);
            void IncreaseRoundRobinIndex();
            void LockExclusiveElementOperations();
            void UnlockExclusiveElementOperations();
            void LockSharedElementOperations();
            void UnlockSharedElementOperations();

    };

} // namespace pandora

#endif