// *************************************
// Pandora Storage Server
// Storage Core
// 'element_container_operations.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "../../server/server_options.h"
#include "../../server/server_utilities.h"
#include "../../server/server_constants.h"
#include "../storage.h"
#include "element_container.h"
#include "main_data.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

namespace pandora {

    namespace core {

        void CreateElementContainer(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::TransactionData& transaction_data) {
    
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            // Lock shared operation
            main_data->LockExclusiveElementContainerOperations();

            // Element Container name
            std::string element_container_name {transaction_data.arguments[pandora::constants::element_container_name]};

            // Check if Element Container already exists
            if(main_data->ElementContainerExists(element_container_name)) {
                transaction_data.log.append("Element Container '" + element_container_name + "' already exists.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerExistsErrorCode, transaction_data);
            }
            
            // Create Element Container
            main_data->AddElementContainer(element_container_name);

            // Log Element Container successful creation
            transaction_data.log.append("Element Container '" + element_container_name + "' was created succesfully.");
            main_data->GetServerOptions()->LogInfo(transaction_data);

            // Unlock shared operation
            main_data->UnlockExclusiveElementContainerOperations();

        }

        void DeleteElementContainer(std::shared_ptr<pandora::MainData>& main_data, pandora::utilities::TransactionData& transaction_data) {
            
            // Check for Element Container in Live Memory
            
            // Check for Element Container in Disk
            // Lock exclusive operation
            main_data->LockExclusiveElementContainerOperations();

            // Element Container name
            std::string element_container_name {transaction_data.arguments[pandora::constants::element_container_name]};

            // Check if Element Container does not exist
            if(!main_data->ElementContainerExists(element_container_name)) {
                transaction_data.log.append("Element Container '" + element_container_name + "' does not exist and could not be deleted.");
                main_data->GetServerOptions()->LogError(pandora::constants::ElementContainerNotExistsErrorCode, transaction_data);
            }

            // Delete Element Container
            main_data->DeleteElementContainer(element_container_name);
            
            // Log Element Container successful deletion
            transaction_data.log.append("Element Container '" + element_container_name + "' was deleted succesfully.");
            main_data->GetServerOptions()->LogInfo(transaction_data);

            // Unlock exclusive operation
            main_data->UnlockExclusiveElementContainerOperations();
            
        }

    }

} // namespace pandora