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

#ifndef INCLUDED_VTGS_FSK96MOD_IMPL_H
#define INCLUDED_VTGS_FSK96MOD_IMPL_H

#include <planck.h>
#include <vtgs/fsk96mod.h>

namespace gr {
  namespace vtgs {

    class fsk96mod_impl : public fsk96mod
    {
     private:
      unsigned int _samp_sym;
      pk_fsk96mod *mod;

     public:
      fsk96mod_impl(unsigned int samp_sym);
      ~fsk96mod_impl();

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_FSK96MOD_IMPL_H */

