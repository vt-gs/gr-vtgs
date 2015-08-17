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


#ifndef INCLUDED_VTGS_AX25_FRAMER_H
#define INCLUDED_VTGS_AX25_FRAMER_H

#include <vtgs/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace vtgs {

    /*!
     * \brief This block frames incoming messages into frames and a byte stream.
     * \ingroup vtgs
     *
     */
    class VTGS_API ax25_framer : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<ax25_framer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of vtgs::ax25_framer.
       *
       * To avoid accidental use of raw pointers, vtgs::ax25_framer's
       * constructor is in a private implementation
       * class. vtgs::ax25_framer::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int padding);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AX25_FRAMER_H */

