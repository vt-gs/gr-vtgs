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

#ifndef INCLUDED_VTGS_FSK96DEMOD_IMPL_H
#define INCLUDED_VTGS_FSK96DEMOD_IMPL_H

#include <planck.h>
#include <vtgs/fsk96demod.h>

namespace gr {
  namespace vtgs {

    class fsk96demod_impl : public fsk96demod
    {
     private:
      pk_fsk96demod *demod;

     public:
      fsk96demod_impl();
      ~fsk96demod_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_FSK96DEMOD_IMPL_H */

