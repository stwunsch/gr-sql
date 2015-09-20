/* -*- c++ -*- */

#define SQL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sql_swig_doc.i"

%{
#include "sql/msg_to_table.h"
%}


%include "sql/msg_to_table.h"
GR_SWIG_BLOCK_MAGIC2(sql, msg_to_table);
