/* -*- c++ -*- */
/*
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_VTGS_AO40_ENCODER_H
#define INCLUDED_VTGS_AO40_ENCODER_H

#include <vtgs/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace vtgs {

    /*!
     * \brief <+description of block+>
     * \ingroup vtgs
     *
     */
    class VTGS_API ao40_encoder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<ao40_encoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of vtgs::ao40_encoder.
       *
       * To avoid accidental use of raw pointers, vtgs::ao40_encoder's
       * constructor is in a private implementation
       * class. vtgs::ao40_encoder::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool continuous, uint32_t flag);
    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AO40_ENCODER_H */
