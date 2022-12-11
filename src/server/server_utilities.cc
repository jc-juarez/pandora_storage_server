// *************************************
// Pandora Storage Server
// Server
// 'server_utilities.cc'
// Author: Juan Carlos Juárez
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include "server_utilities.h"
#include "server_options.h"
#include <httpserver.hpp>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <ctime>

namespace pandora {

    namespace server_utilities {

        static std::random_device rd;
        static std::mt19937 seed(rd());

        void ConsoleLog(const std::string log) {
            if(server_options::debug_enabled) std::cout << "<> " << log << "\n";
        }

        void CreateDirectory(const std::filesystem::path path) {
            try {
                // Create Pandora Storage Server directory
                std::filesystem::create_directories(path);
            } catch(...) {
                std::string error {};
                error.append("Pandora could not create the " + std::string(path) + " directory.");
                throw std::runtime_error(error);
            }
        }

        DateTime GetDateTime() {
            int two_digits {2};
            int initial_position {0};

            time_t now = time(0);
            tm *ltm = localtime(&now);
            
            DateTime dt;
            dt.year = std::to_string(1900 + ltm->tm_year);
            dt.month = std::to_string(1 + ltm->tm_mon);
            dt.day = std::to_string(ltm->tm_mday);
            dt.hours = std::to_string(5+ltm->tm_hour);
            dt.minutes = std::to_string(30+ltm->tm_min);
            dt.seconds = std::to_string(ltm->tm_sec);

            // Care for two digits for all data
            if(dt.month.size() < two_digits) dt.month.insert(initial_position, "0");
            if(dt.day.size() < two_digits) dt.day.insert(initial_position, "0");
            if(dt.hours.size() < two_digits) dt.hours.insert(initial_position, "0");
            if(dt.minutes.size() < two_digits) dt.minutes.insert(initial_position, "0");
            if(dt.seconds.size() < two_digits) dt.seconds.insert(initial_position, "0");

            return dt;
        }

        std::string GetRandomString_Size8() {
            std::uniform_int_distribution<int> range(10000000, 99999999);
            return std::to_string(range(seed)); 
        }

        std::string GenerateTransactionID() {
            std::string transaction_id {};
            transaction_id.append(pandora::server_utilities::GetRandomString_Size8() + "-" + pandora::server_utilities::GetRandomString_Size8());
            return std::move(transaction_id);
        }

    }

} // namespace pandora