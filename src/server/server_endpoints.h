// *************************************
// Pandora Storage Server
// Server
// 'server_endpoints.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_ENDPOINTS_H
#define SERVER_ENDPOINTS_H

#include "../storage/core/live-memory/element_container_cache.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <memory>

namespace pandora {

    namespace server_endpoints {

        // Resource not found
        std::shared_ptr<httpserver::http_response> resource_not_found(const httpserver::http_request&);

        // HTTP Method not allowed
        std::shared_ptr<httpserver::http_response> method_not_allowed(const httpserver::http_request&);

        // Create Element Container
        class CreateElementContainerEndpoint : public httpserver::http_resource {

            // Constructor
            public:
                CreateElementContainerEndpoint(std::shared_ptr<pandora::ElementContainerCache>&, pandora::ServerOptions*);

            public:
                std::shared_ptr<httpserver::http_response> render_PUT(const httpserver::http_request&);
                std::shared_ptr<pandora::ElementContainerCache> m_main_cache;
                pandora::ServerOptions* m_server_options;
        };

        // Delete Element Container
        class DeleteElementContainerEndpoint : public httpserver::http_resource {
            public:
                std::shared_ptr<httpserver::http_response> render_DELETE(const httpserver::http_request&);
        };

        // Set Element
        class SetElementEndpoint : public httpserver::http_resource {
            public:
                std::shared_ptr<httpserver::http_response> render_POST(const httpserver::http_request&);
        };

        // Get Element
        class GetElementEndpoint : public httpserver::http_resource {
            public:
                std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request&);
        };

        // Delete Element
        class DeleteElementEndpoint : public httpserver::http_resource {
            public:
                std::shared_ptr<httpserver::http_response> render_DELETE(const httpserver::http_request&);
        };

    }

} // namespace pandora

#endif
