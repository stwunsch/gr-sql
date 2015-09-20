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

#ifndef INCLUDED_SQL_MSG_TO_TABLE_IMPL_H
#define INCLUDED_SQL_MSG_TO_TABLE_IMPL_H

#include <sql/msg_to_table.h>

#include <mysql.h>

namespace gr {
  namespace sql {

    class msg_to_table_impl : public msg_to_table
    {
     private:
      // Nothing to declare in this block.

     public:
      msg_to_table_impl(std::string user, std::string password, std::string database, std::string table, std::string host, unsigned int port);
      ~msg_to_table_impl();
      void handle_msg(pmt::pmt_t msg);

      MYSQL *d_mysql;
      std::string d_table;

      pmt::pmt_t d_port_id_in;
    };

  } // namespace sql
} // namespace gr

#endif /* INCLUDED_SQL_MSG_TO_TABLE_IMPL_H */

