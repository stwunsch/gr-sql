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

namespace gr {
  namespace sql {

    msg_to_table::sptr
    msg_to_table::make(std::string user, std::string password, std::string database, std::string table)
    {
      return gnuradio::get_initial_sptr
        (new msg_to_table_impl(user, password, database, table));
    }

    /*
     * The private constructor
     */
    msg_to_table_impl::msg_to_table_impl(std::string user, std::string password, std::string database, std::string table)
      : gr::block("msg_to_table",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        // Initialize mysql instance
        d_mysql = NULL;
        d_mysql = mysql_init(d_mysql);

        if(!d_mysql) throw std::runtime_error("Initialize mysql instance failed.");
    }

    /*
     * Our virtual destructor.
     */
    msg_to_table_impl::~msg_to_table_impl()
    {
    }

  } /* namespace sql */
} /* namespace gr */

