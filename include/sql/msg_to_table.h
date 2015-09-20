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


#ifndef INCLUDED_SQL_MSG_TO_TABLE_H
#define INCLUDED_SQL_MSG_TO_TABLE_H

#include <sql/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace sql {

    /*!
     * \brief <+description of block+>
     * \ingroup sql
     *
     */
    class SQL_API msg_to_table : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_to_table> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sql::msg_to_table.
       *
       * To avoid accidental use of raw pointers, sql::msg_to_table's
       * constructor is in a private implementation
       * class. sql::msg_to_table::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string user, std::string password, std::string database, std::string table);
    };

  } // namespace sql
} // namespace gr

#endif /* INCLUDED_SQL_MSG_TO_TABLE_H */

