#pragma once

#include "Location.hpp"

namespace ws {
  class Server {
  public:
    typedef std::pair<u_int32_t, u_int16_t> listen_type;
    typedef std::vector<listen_type> listen_vec_type;
    typedef std::string server_name_type;
    typedef std::vector<server_name_type> server_name_vec_type;
    typedef std::pair<std::string, ws::Location> location_pair_type;
    typedef std::map<location_pair_type::first_type, location_pair_type::second_type> location_map_type;
    typedef ws::InnerOption::autoindex_type autoindex_type;
    typedef ws::InnerOption::root_type root_type;
    typedef ws::InnerOption::index_set_type index_set_type;
    typedef ws::InnerOption::index_type index_type;
    typedef ws::InnerOption::index_check_type index_check_type;
    typedef ws::InnerOption::client_max_body_size_type client_max_body_size_type;
    typedef ws::InnerOption::error_page_map_type error_page_map_type;
    typedef ws::InnerOption::error_page_type error_page_type;

  private:
    listen_vec_type _listen_vec;
    server_name_vec_type _server_name_vec;
    location_map_type _location_map;
    ws::InnerOption _option;

  public:
    Server();
    explicit Server(const Server& other);
    Server& operator=(const Server& other);
    ~Server();

    const listen_vec_type& get_listen_vec() const throw();
    const server_name_vec_type& get_server_name_vec() const throw();
    const location_map_type& get_location_map() const throw();
    const ws::InnerOption& get_option() const throw();
    
    const autoindex_type& get_autoindex() const throw();
    const root_type& get_root() const throw();
    const index_set_type& get_index_set() const throw();
    index_check_type get_index_check() const throw();
    const client_max_body_size_type& get_client_max_body_size() const throw();

    void add_listen(const listen_type& value);
    void add_server_name(const server_name_type& value);

    void set_location_map(const location_map_type& value);
    void set_option(const ws::InnerOption& value);

    void set_autoindex(const autoindex_type& value);
    void set_root(const root_type& value);
    void add_index(const index_type& value);
    void set_index_check(index_check_type value);
    void set_client_max_body_size(const client_max_body_size_type& value);

    const Location& find_location(location_pair_type::first_type dir) const throw();
  };
}
