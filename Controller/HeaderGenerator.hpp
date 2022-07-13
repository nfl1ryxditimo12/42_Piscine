#pragma once

#include <string>

#include "Socket.hpp"

namespace ws {
  class HeaderGenerator {
  public:
    typedef ws::Location::limit_except_vec_type limit_except_vec_type;
    typedef ws::Socket::client_value_type client_value_type;

  private:
    static void generate_start_line(std::string& data, unsigned int stat);
    static void generate_representation_header(
      std::string& data, const client_value_type& client_data, std::string::size_type content_length
    );
    static void generate_response_header(std::string& data, const client_value_type& client_data);
    static void generate_connection_header(std::string& data, const client_value_type& client_data);

    static void generate_content_type_line(std::string& data);
    static void generate_content_length_line(std::string& data, std::string::size_type content_length);
    static void generate_transfer_encoding_line(std::string& data, const client_value_type& client_data);

    static void generate_date_line(std::string& data);
    static void generate_server_line(std::string& data);
    static void generate_allow_line(std::string& data, const client_value_type& client_data);
    
    static void generate_connection_line(std::string& data);

    HeaderGenerator();
    ~HeaderGenerator();
    HeaderGenerator(const HeaderGenerator& other);
    HeaderGenerator& operator=(const HeaderGenerator& other);

  public:
    static std::string generate(const client_value_type& client_data, std::string::size_type content_length);
  };
}
