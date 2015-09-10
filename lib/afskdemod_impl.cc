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
#include "afskdemod_impl.h"

namespace gr {
  namespace vtgs {

    afskdemod::sptr
    afskdemod::make(unsigned int samp_sym, unsigned int baud, float mark_freq, float space_freq)
    {
      return gnuradio::get_initial_sptr
        (new afskdemod_impl(samp_sym, baud, mark_freq, space_freq));
    }

    /*
     * The private constructor
     */
    afskdemod_impl::afskdemod_impl(unsigned int samp_sym, unsigned int baud, float mark_freq, float space_freq)
      : gr::block("afskdemod",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        _samp_sym = samp_sym;

        demod = pk_bfskdemod_create(samp_sym, baud, mark_freq, space_freq);
    }

    /*
     * Our virtual destructor.
     */
    afskdemod_impl::~afskdemod_impl()
    {
        pk_bfskdemod_destroy(demod);
    }

    void
    afskdemod_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items * _samp_sym;
    }

    int
    afskdemod_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        pk_bfskdemod_process(demod, in, noutput_items);

        size_t nitems = 0;
        unsigned char *data = pk_bfskdemod_read(demod, &nitems);
        memcpy(out, data, nitems * sizeof(unsigned char));

        consume_each (noutput_items);
        return nitems;
    }

  } /* namespace vtgs */
} /* namespace gr */

