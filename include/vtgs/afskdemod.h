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


#ifndef INCLUDED_VTGS_AFSKDEMOD_H
#define INCLUDED_VTGS_AFSKDEMOD_H

#include <vtgs/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace vtgs {

    /*!
     * \brief <+description of block+>
     * \ingroup vtgs
     *
     */
    class VTGS_API afskdemod : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<afskdemod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of vtgs::afskdemod.
       *
       * To avoid accidental use of raw pointers, vtgs::afskdemod's
       * constructor is in a private implementation
       * class. vtgs::afskdemod::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int samp_sym, unsigned int baud, float mark_freq, float space_freq);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AFSKDEMOD_H */

