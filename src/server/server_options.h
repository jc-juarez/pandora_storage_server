// *************************************
// Pandora Storage Server
// Server
// 'server_options.h'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H

#include <string>

namespace pandora {

    namespace server_options {

        extern int port_number;
        extern bool debug_enabled;
        extern bool logs_enabled;
        extern bool display_requests;
        extern std::string server_session_id;
        extern std::string logs_file_path;

    }

} // namespace pandora

#endif