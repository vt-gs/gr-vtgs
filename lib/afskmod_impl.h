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

#ifndef INCLUDED_VTGS_AFSKMOD_IMPL_H
#define INCLUDED_VTGS_AFSKMOD_IMPL_H

#include <planck.h>
#include <vtgs/afskmod.h>

namespace gr {
  namespace vtgs {

    class afskmod_impl : public afskmod
    {
     private:
      unsigned int _samp_sym;
      pk_bfskmod *mod;

     public:
      afskmod_impl(
        unsigned int samp_sym, 
        unsigned int baud, 
        float mark_freq, 
        float space_freq
      );

      ~afskmod_impl();

      int work(int noutput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AFSKMOD_IMPL_H */

