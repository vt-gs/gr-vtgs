/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_VTGS_FSK96MOD_H
#define INCLUDED_VTGS_FSK96MOD_H

#include <vtgs/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace vtgs {

    /*!
     * \brief <+description of block+>
     * \ingroup vtgs
     *
     */
    class VTGS_API fsk96mod : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<fsk96mod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of vtgs::fsk96mod.
       *
       * To avoid accidental use of raw pointers, vtgs::fsk96mod's
       * constructor is in a private implementation
       * class. vtgs::fsk96mod::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int samp_sym);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_FSK96MOD_H */

