/* -*- c++ -*- */
/* Copyright 2015 Stefan Wunsch
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "msg_to_table_impl.h"

#include <iostream>

namespace gr {
  namespace sql {

    msg_to_table::sptr
    msg_to_table::make(std::string user, std::string password, std::string database, std::string table, std::string host, unsigned int port)
    {
      return gnuradio::get_initial_sptr
        (new msg_to_table_impl(user, password, database, table, host, port));
    }

    /*
     * The private constructor
     */
    msg_to_table_impl::msg_to_table_impl(std::string user, std::string password, std::string database, std::string table, std::string host, unsigned int port)
      : gr::block("msg_to_table",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        // Initialize mysql instance
        d_mysql = NULL;
        d_mysql = mysql_init(d_mysql);

        if(!d_mysql) throw std::runtime_error("Initialize mysql instance failed.");

        // Connect to Mysql server
        if(!mysql_real_connect(d_mysql,      /* MYSQL structure to use */
                           host.c_str(),     /* server hostname or IP address */
                           user.c_str(),     /* mysql user */
                           password.c_str(), /* password */
                           database.c_str(), /* default database to use, NULL for none */
                           port,                /* port number, 0 for default */
                           NULL,             /* socket file or named pipe name */
                           CLIENT_FOUND_ROWS /* connection flags */ )){
            std::string error_msg = "Connection to MySQL server failed: ";
            error_msg.append(mysql_error(d_mysql));
            throw std::runtime_error(error_msg.c_str());
        }
        else{
            std::cout << "Connection to MySQL server established." << std::endl;
            std::cout << "-> Selected user: " << user << std::endl;
            std::cout << "-> Selected database: " << database << std::endl;
            std::cout << "-> MySQL Server Info: " << mysql_get_server_info(d_mysql) << std::endl;
            std::cout << "-> MySQL Client Info: " << mysql_get_client_info() << std::endl;
        }

        // Init msg ports

        // Test insert to given table
        MYSQL_STMT* stmt = mysql_stmt_init(d_mysql);
        std::string query = "INSERT INTO my_tab (id,value) VALUES (17,42.42);";
        if(mysql_stmt_prepare(stmt, query.c_str(), query.size())){
            std::string error_msg = "SQL query init failed: ";
            error_msg.append(mysql_stmt_error(stmt));
            error_msg.append(" : ");
            error_msg.append(query);
            throw std::runtime_error(error_msg.c_str());
        }
        if(mysql_stmt_execute(stmt)){
            std::string error_msg = "SQL query execution failed: ";
            error_msg.append(mysql_stmt_error(stmt));
            error_msg.append(" : ");
            error_msg.append(query);
            throw std::runtime_error(error_msg.c_str());
        }
        mysql_stmt_close(stmt);

    }

    /*
     * Our virtual destructor.
     */
    msg_to_table_impl::~msg_to_table_impl()
    {
        mysql_close(d_mysql);
    }

  } /* namespace sql */
} /* namespace gr */

