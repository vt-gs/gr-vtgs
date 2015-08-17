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

#include <gnuradio/io_signature.h>
#include "ax25_deframer_impl.h"

namespace gr {
  namespace vtgs {

    using namespace blocks;

    ax25_deframer::sptr
    ax25_deframer::make()
    {
      return gnuradio::get_initial_sptr
        (new ax25_deframer_impl());
    }

    void ax25_deframer_impl::callback(
        int valid,
        unsigned char *payload,
        void *ref_info,
        size_t size)
    {
        ax25_deframer_impl *object = (ax25_deframer_impl *) ref_info;
        object->output_pdu(valid, payload, size);
    }

    void ax25_deframer_impl::output_pdu(
        int valid,
        unsigned char *payload,
        size_t size)
    {
        pmt::pmt_t frame_vector;
        pmt::pmt_t frame_id = pmt::PMT_NIL;
        pmt::pmt_t frame_pair;

        frame_vector = pdu::make_pdu_vector(
            pdu::byte_t, payload, size
        );

        frame_pair = pmt::cons(frame_id, frame_vector);

        if (valid)
            message_port_pub(pmt::mp("valid_frames"), frame_pair);
        else
            message_port_pub(pmt::mp("invalid_frames"), frame_pair);
    }

    /*
     * The private constructor
     */
    ax25_deframer_impl::ax25_deframer_impl()
      : gr::block("ax25_deframer",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(0, 0, 0))
    {
        deframer = pk_ax25_deframer_create(this, ax25_deframer_impl::callback);

        message_port_register_out(pmt::mp("valid_frames"));
        message_port_register_out(pmt::mp("invalid_frames"));
    }

    /*
     * Our virtual destructor.
     */
    ax25_deframer_impl::~ax25_deframer_impl()
    {
        pk_ax25_deframer_destroy(deframer);
    }

    int
    ax25_deframer_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        int ninput = ninput_items[0];

        pk_ax25_deframer_process(deframer, in, ninput);

        consume_each (ninput);
        return 0;
    }

  } /* namespace vtgs */
} /* namespace gr */

