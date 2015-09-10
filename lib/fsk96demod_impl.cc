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
#include "fsk96demod_impl.h"

namespace gr {
  namespace vtgs {

    fsk96demod::sptr
    fsk96demod::make(unsigned int samp_sym)
    {
      return gnuradio::get_initial_sptr
        (new fsk96demod_impl(samp_sym));
    }

    /*
     * The private constructor
     */
    fsk96demod_impl::fsk96demod_impl(unsigned int samp_sym)
      : gr::block("fsk96demod",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        _samp_sym = samp_sym;

        demod = pk_fsk96demod_create(_samp_sym);
    }

    /*
     * Our virtual destructor.
     */
    fsk96demod_impl::~fsk96demod_impl()
    {
        pk_fsk96demod_destroy(demod);
    }

    void
    fsk96demod_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items * _samp_sym;
    }

    int
    fsk96demod_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        pk_fsk96demod_process(demod, in, noutput_items);

        size_t nitems = 0;
        unsigned char *data = pk_fsk96demod_read(demod, &nitems);
        memcpy(out, data, nitems * sizeof(unsigned char));

        consume_each (noutput_items);
        return nitems;
    }

  } /* namespace vtgs */
} /* namespace gr */

