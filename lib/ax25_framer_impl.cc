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
#include "ax25_framer_impl.h"

namespace gr {
  namespace vtgs {

    ax25_framer::sptr
    ax25_framer::make(unsigned int padding)
    {
      return gnuradio::get_initial_sptr
        (new ax25_framer_impl(padding));
    }

    /*
     * The private constructor
     */
    ax25_framer_impl::ax25_framer_impl(unsigned int padding)
      : gr::sync_block("ax25_framer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        framer = pk_ax25_framer_create(padding);

        d_offset = 0;

        message_port_register_in(pmt::mp("in"));
        set_msg_handler(pmt::mp("in"), boost::bind(&ax25_framer_impl::pmt_in, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    ax25_framer_impl::~ax25_framer_impl()
    {
        pk_ax25_framer_destroy(framer);
    }

    void ax25_framer_impl::pmt_in(pmt::pmt_t msg)
    {
        msg_queue.push(msg);
    }

    int
    ax25_framer_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        unsigned char *out = (unsigned char *) output_items[0];

        // read the frame contents in bits
        size_t nitems;
        unsigned char *frame_data = pk_ax25_framer_read(framer, &nitems);

        int items_left = static_cast<int>(nitems) - d_offset;
        if (items_left > 0) {
            size_t noutput = std::min(items_left, noutput_items);
            for (size_t i = 0; i < noutput; i++) {
                out[i] = frame_data[d_offset];
                d_offset++;
            }

            return noutput;
        }

        if (msg_queue.size() > 0) {
            pmt::pmt_t msg = msg_queue.front();
            msg_queue.pop();

            if (pmt::is_pair(msg)) {

                // unwrap the PDU vector
                pmt::pmt_t meta = pmt::car(msg);
                pmt::pmt_t vect = pmt::cdr(msg);
                size_t size = pmt::length(vect);

                size_t io;
                unsigned char *data;
                data = (unsigned char *) uniform_vector_elements(vect, io);

                pk_ax25_framer_process(framer, data, size);

                d_offset = 0;
            }
        }


        return 0;
    }

  } /* namespace vtgs */
} /* namespace gr */

