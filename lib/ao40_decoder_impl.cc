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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include <gnuradio/io_signature.h>

#include "ao40_fec_decoder.h"
#include "ao40_decoder_impl.h"

#define AO40_CODED_SIZE 5200
#define AO40_CODED_BYTE 650
#define AO40_FRAME_SIZE 2048
#define AO40_FRAME_BYTE 256
#define AO40_MARKER     0x1acffc1d

namespace gr {
  namespace vtgs {

    using namespace blocks;

    ao40_decoder::sptr
    ao40_decoder::make()
    {
      return gnuradio::get_initial_sptr
        (new ao40_decoder_impl());
    }

    /*
     * The private constructor
     */
    ao40_decoder_impl::ao40_decoder_impl()
      : gr::block("ao40_decoder",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(0, 0, 0))
    {
        debug = true;
        decoder = new ao40_fec_decoder();

        state = DETECT;
        marker_buf = 0;

        d_offset = 0;

        coded  = new unsigned char[AO40_CODED_SIZE];
        packed = new unsigned char[AO40_FRAME_BYTE];

        message_port_register_out(pmt::mp("valid_frames"));
        message_port_register_out(pmt::mp("invalid_frames"));
    }

    void ao40_decoder_impl::output_pdu(int errors)
    {
        pmt::pmt_t frame_vector;
        pmt::pmt_t frame_id = pmt::PMT_NIL;
        pmt::pmt_t frame_pair;

        frame_vector = pdu::make_pdu_vector(
            pdu::byte_t,
            packed,
            AO40_FRAME_BYTE
        );

        frame_pair = pmt::cons(frame_id, frame_vector);

        bool valid = errors < 1040;
        if (valid)
            message_port_pub(pmt::mp("valid_frames"), frame_pair);
        else
            message_port_pub(pmt::mp("invalid_frames"), frame_pair);
    }

    /*
     * Our virtual destructor.
     */
    ao40_decoder_impl::~ao40_decoder_impl()
    {
        delete decoder;

        delete [] coded;
        delete [] packed;
    }

    int
    ao40_decoder_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        unsigned char *in = (unsigned char *) input_items[0];
        int ninput = ninput_items[0];

        if (state == DETECT) {
            for (size_t i = 0; i < ninput; i++) {

                // invert the input
                in[i] = (in[i] & 1) == 1 ? 0 : 1;

                marker_buf <<= 1;
                marker_buf &= 0xFFFFFFFE;
                marker_buf |= (in[i] & 1);

                if (marker_buf == AO40_MARKER) {
                    if (debug) {
                        std::cout << "***** ao40_decoder debug *****" << std::endl;
                        std::cout << "     Detected a marker!" << std::endl;
                    }

                    state = DECODE;
                    d_offset = 0;

                    consume_each(i+1);
                    return 0;
                }
            }
        } else if (state == DECODE) {
            for (size_t i = 0; i < ninput; i++) {

                if (d_offset < AO40_CODED_SIZE) {
                    coded[d_offset++] = (in[i] & 1) == 1 ? 0x00 : 0xFF;
                } else {
                    state = DETECT;

                    int errors = decoder->ao40_decode(packed, coded, 1);
                    if (debug) {
                        std::cout << "***** ao40_decoder debug *****" << std::endl;
                        std::cout << "     Errors corrected:    ";
                        std::cout << float(errors) * 100.0f / float(AO40_CODED_SIZE);
                        std::cout << "%" << std::endl;
                    }

                    output_pdu(errors);

                    consume_each(i+1);
                    return 0;
                }
            }
        }

        consume_each (ninput);
        return 0;
    }

  } /* namespace vtgs */
} /* namespace gr */

