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
#include "fsk96mod_impl.h"

namespace gr {
  namespace vtgs {

    fsk96mod::sptr
    fsk96mod::make(unsigned int samp_sym)
    {
      return gnuradio::get_initial_sptr
        (new fsk96mod_impl(samp_sym));
    }

    /*
     * The private constructor
     */
    fsk96mod_impl::fsk96mod_impl(unsigned int samp_sym)
      : gr::sync_interpolator("fsk96mod",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(float)),
              samp_sym)
    {
        _samp_sym = samp_sym;

        mod = pk_fsk96mod_create(samp_sym);
    }

    /*
     * Our virtual destructor.
     */
    fsk96mod_impl::~fsk96mod_impl()
    {
        pk_fsk96mod_destroy(mod);
    }

    int
    fsk96mod_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        float *out = (float *) output_items[0];

        for (size_t i = 0; i < noutput_items / _samp_sym; i++) 
            pk_fsk96mod_execute(mod, &out[i * _samp_sym], in[i]);

        return noutput_items;
    }

  } /* namespace vtgs */
} /* namespace gr */

