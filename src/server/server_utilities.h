// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_UTILITIES_H
#define SERVER_UTILITIES_H

#include "../storage/core/live-memory/element_container_cache.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <filesystem>
#include <string>

namespace pandora {

    namespace server_utilities {

        struct DateTime {
            std::string year {};
            std::string month {};
            std::string day {};
            std::string hours {};
            std::string minutes {};
            std::string seconds {};
        };

        void CreateDirectory(const std::filesystem::path);
        void SetEndpoints(httpserver::webserver&, pandora::ElementContainerCache&, pandora::ServerOptions&);
        void CreateBaseDirectories();
        DateTime GetDateTime();
        std::string GetRandomString_Size8();
        std::string GenerateServerSessionID();
        std::string GenerateTransactionID();

        // Functions Templates
        template <typename T> 
        void RegisterEndpoint(httpserver::webserver& pandora_storage_server, T& endpoint_resource, const std::string http_method, const std::string endpoint_url) {
            endpoint_resource.disallow_all();
            endpoint_resource.set_allowing(http_method, true);
            pandora_storage_server.register_resource(endpoint_url, &endpoint_resource);
        }

    }

} // namespace pandora

#endif
