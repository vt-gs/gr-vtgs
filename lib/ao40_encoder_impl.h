/* -*- c++ -*- */
/*
 * Copyright 2016 Virginia Tech.
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

#ifndef INCLUDED_VTGS_AO40_ENCODER_IMPL_H
#define INCLUDED_VTGS_AO40_ENCODER_IMPL_H

#include <vtgs/ao40_encoder.h>
#include <gnuradio/blocks/pdu.h>
#include "ao40_fec_decoder.h"
#include <queue>

namespace gr {
  namespace vtgs {

    class ao40_encoder_impl : public ao40_encoder
   {
     private:
      // Debug variables
      bool debug;
      int messages_received;
      int messages_transmitted;
      bool continuous;
      uint32_t flag;

      ao40_fec_encoder *encoder;

      uint8_t marker_buf[32];

      /***** Message handling *****/
      void handler (const pmt::pmt_t msg);
      std::queue<std::vector<uint8_t> > queue;

     public:
      ao40_encoder_impl(bool continuous, uint32_t flag);
      ~ao40_encoder_impl();

      // void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AO40_ENCODER_IMPL_H */
