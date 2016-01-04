#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2015 Stefan Wunsch
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import sql_swig as sql
import pmt
from time import sleep

class qa_msg_to_table (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up message

        msg1 = pmt.list3(pmt.string_to_symbol('id'),pmt.string_to_symbol('int'),pmt.from_long(42))
        msg2 = pmt.list3(pmt.string_to_symbol('value'),pmt.string_to_symbol('float'),pmt.from_float(3.1416))
        #msg3 = pmt.list3(pmt.string_to_symbol('text'),pmt.string_to_symbol('string'),pmt.string_to_symbol('some text'))
        msg = pmt.list2(msg1,msg2)

        # set up sql connection

        host = '127.0.0.1'
        port = 0 # default
        user = 'my_user'
        password = 'my_password'
        database = 'my_db'
        table = 'my_table'

        # set up flowgraph

        if 0: # Enable and disable here
            msg_src = blocks.message_strobe(msg,100)
            sql_connector = sql.msg_to_table(user,password,database,table,host,port)

            self.tb.msg_connect(msg_src,'strobe',sql_connector,'Msg in')

        # run flowgraph

        self.tb.start()
        sleep(0.2)
        self.tb.stop()
        self.tb.wait()


if __name__ == '__main__':
    gr_unittest.run(qa_msg_to_table)#, "qa_msg_to_table.xml")
