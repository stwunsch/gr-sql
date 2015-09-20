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
        d_table = table;

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
            std::string error_msg = "[gr-sql] Connection to MySQL server failed: ";
            error_msg.append(mysql_error(d_mysql));
            throw std::runtime_error(error_msg.c_str());
        }
        else{
            std::cout << "[gr-sql] Connection to MySQL server established." << std::endl;
            std::cout << "[gr-sql] -> Selected user: " << user << std::endl;
            std::cout << "[gr-sql] -> Selected database: " << database << std::endl;
            std::cout << "[gr-sql] -> MySQL Server Info: " << mysql_get_server_info(d_mysql) << std::endl;
            std::cout << "[gr-sql] -> MySQL Client Info: " << mysql_get_client_info() << std::endl;
        }

		// Register input message port
		d_port_id_in = pmt::mp("Msg in");
		message_port_register_in(d_port_id_in);
		set_msg_handler(d_port_id_in, boost::bind(&msg_to_table_impl::handle_msg, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    msg_to_table_impl::~msg_to_table_impl()
    {
        mysql_close(d_mysql);
    }

    void
    msg_to_table_impl::handle_msg(pmt::pmt_t msg)
    {
        std::vector<std::string> symbols;
        std::vector<std::string> values;

        unsigned int msg_size = pmt::length(msg);

        // Go through message parts
        for(int k=0; k<msg_size; k++){
            pmt::pmt_t msg_part = pmt::nth(k,msg);

            // Error handling
            if(pmt::length(msg_part)!=3){
                std::cout << "[gr-sql] Not readable msg part found (message has wrong size)." << std::endl;
                continue;
            }
            if(not(pmt::is_symbol(pmt::nth(0,msg_part))&&pmt::is_symbol(pmt::nth(1,msg_part)))){
                std::cout << "[gr-sql] Not readable msg part found (message has wrong composition)." << std::endl;
                continue;
            }

            // Read msg part
            std::string datatype = pmt::symbol_to_string(pmt::nth(1,msg_part));
            std::string value;
            if(datatype=="int"){
                std::stringstream ss;
                ss << pmt::to_long(pmt::nth(2,msg_part));
                value = ss.str();
            }
            else if(datatype=="float"){
                std::stringstream ss;
                ss << pmt::to_float(pmt::nth(2,msg_part));
                value = ss.str();
            }
            else if(datatype=="string"){
                value = "'";
                value.append(pmt::symbol_to_string(pmt::nth(2,msg_part)));
                value.append("'");
            }
            else{
                std::cout << "[gr-sql] Not readable msg part found (unknown data type)." << std::endl;
                continue;
            }

            // Add msg part to list for building SQL statement
            symbols.push_back(pmt::symbol_to_string(pmt::nth(0,msg_part)));
            values.push_back(value);
        }

        if(symbols.size()!=0){
            // Compose message for SQL statement
            std::string str_ids, str_values;
            for(int k=0; k<symbols.size(); k++){
                if(k<symbols.size()-1){
                    str_ids.append(symbols[k]);
                    str_ids.append(",");
                    str_values.append(values[k]);
                    str_values.append(",");
                }
                else{
                    str_ids.append(symbols[k]);
                    str_values.append(values[k]);
                }
            }

            // Send msg to SQL database
            MYSQL_STMT* stmt = mysql_stmt_init(d_mysql);
            std::string query = "INSERT INTO ";
            query.append(d_table);
            query.append(" (");
            query.append(str_ids);
            query.append(") VALUES (");
            query.append(str_values);
            query.append(");");
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
        else{
            std::cout << "[gr-sql] Empty message processed." << std::endl;
        }
    }

  } /* namespace sql */
} /* namespace gr */

