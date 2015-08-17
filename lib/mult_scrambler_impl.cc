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
#include "mult_scrambler_impl.h"

namespace gr {
  namespace vtgs {

    mult_scrambler::sptr
    mult_scrambler::make(unsigned int n, uint32_t start)
    {
      return gnuradio::get_initial_sptr
        (new mult_scrambler_impl(n, start));
    }

    /*
     * The private constructor
     */
    mult_scrambler_impl::mult_scrambler_impl(unsigned int n, uint32_t start)
      : gr::sync_block("mult_scrambler",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        scrambler = pk_mult_scrambler_create(n, start);
    }

    /*
     * Our virtual destructor.
     */
    mult_scrambler_impl::~mult_scrambler_impl()
    {
        pk_mult_scrambler_destroy(scrambler);
    }

    int
    mult_scrambler_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        for (size_t i = 0; i < noutput_items; i++)
            out[i] = pk_mult_scrambler_execute(scrambler, in[i]);

        return noutput_items;
    }

  } /* namespace vtgs */
} /* namespace gr */
