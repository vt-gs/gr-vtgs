/* -*- c++ -*- */
/* 
 * Copyright 2015 Virginia Tech
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

#ifndef INCLUDED_VTGS_AO40_DECODER_IMPL_H
#define INCLUDED_VTGS_AO40_DECODER_IMPL_H

#include <vtgs/ao40_decoder.h>
#include <gnuradio/blocks/pdu.h>
#include "ao40_fec_decoder.h"

namespace gr {
  namespace vtgs {

    class ao40_decoder_impl : public ao40_decoder
    {
     private:
      bool debug;
      ao40_fec_decoder *decoder;
     
      enum decoder_state {
         DETECT=0,
         DECODE
      };

      decoder_state state;

      uint32_t marker_buf;
      size_t d_offset;

      unsigned char *coded;
      unsigned char *packed;

      void output_pdu(int errors);

     public:
      ao40_decoder_impl();
      ~ao40_decoder_impl();

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AO40_DECODER_IMPL_H */

