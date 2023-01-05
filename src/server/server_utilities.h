// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_UTILITIES_H
#define SERVER_UTILITIES_H

#include <httpserver.hpp>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <string>
#include <chrono>

namespace pandora {

    class MainData;
    class ServerOptions;

    namespace utilities {

        struct RequestData {

            std::string transaction_id {};
            std::string request_path {};
            std::string http_method {};
            std::string log {};
            std::string ellapsed_milliseconds {};
            std::unordered_map<std::string, std::string> arguments {};
            std::stringstream logs_stream {};
            std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point;
            RequestData(std::string _transaction_id, std::string _request_path, std::string _http_method, std::chrono::time_point<std::chrono::high_resolution_clock> _start_time_point) : 
                        transaction_id(_transaction_id), request_path(_request_path), http_method(_http_method), start_time_point(_start_time_point) {}
        
        };

        struct DateTime {
            std::string year {};
            std::string month {};
            std::string day {};
            std::string hours {};
            std::string minutes {};
            std::string seconds {};
        };

        void SetEndpoints(httpserver::webserver&, std::shared_ptr<pandora::MainData>&);
        void CreateBaseDirectories();
        void ValidateElementContainerName(RequestData&, pandora::ServerOptions*);
        void ValidateElementID(RequestData&, pandora::ServerOptions*);
        void ValidateElementValue(RequestData&, pandora::ServerOptions*);
        DateTime GetDateTime();
        std::string GetRandomString_Size9();
        std::string GenerateTransactionID();
        std::string GetDateTimeString();
        std::string GetEllapsedMillisecondsString(std::chrono::time_point<std::chrono::high_resolution_clock>&,
                                                  std::chrono::time_point<std::chrono::high_resolution_clock>&);

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
