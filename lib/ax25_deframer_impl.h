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

#ifndef INCLUDED_VTGS_AX25_DEFRAMER_IMPL_H
#define INCLUDED_VTGS_AX25_DEFRAMER_IMPL_H

#include <planck.h>
#include <gnuradio/blocks/pdu.h>
#include <vtgs/ax25_deframer.h>

namespace gr {
  namespace vtgs {

    class ax25_deframer_impl : public ax25_deframer
    {
     private:
      pk_ax25_deframer *deframer;

     public:
      ax25_deframer_impl();
      ~ax25_deframer_impl();

      void output_pdu(
        int valid, 
        unsigned char *payload, 
        size_t size
      );

      static void callback(
        int valid, 
        unsigned char *payload, 
        void *ref_info, 
        size_t size
      );

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AX25_DEFRAMER_IMPL_H */

