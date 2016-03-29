/* -*- c++ -*- */
/*
 * Copyright 2016 Virginia Tech
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

#include <iostream>

#include <gnuradio/io_signature.h>

#include "ao40_encoder_impl.h"
#include "ao40_fec_decoder.h"

#define AO40_CODED_SIZE 5200
#define AO40_CODED_BYTE 650
#define AO40_FRAME_SIZE 2048
#define AO40_FRAME_BYTE 256
#define AO40_MARKER     0x1acffc1d

namespace gr {
  namespace vtgs {

    using namespace blocks;

    ao40_encoder::sptr
    ao40_encoder::make(bool continuous, uint32_t flag)
    {
      return gnuradio::get_initial_sptr
        (new ao40_encoder_impl(continuous, flag));
    }

    ao40_encoder_impl::ao40_encoder_impl(bool param_continuous, uint32_t param_flag)
      : gr::block("ao40_encoder",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        debug = true;
        messages_received = 0;
        messages_transmitted = 0;
        continuous = param_continuous;

        encoder = new ao40_fec_encoder();

        // Populate the marker buffer
        flag = param_flag;
        for (int i = 0; i < 32; i++) {
          marker_buf[31-i] = (flag>>i) & 0x01;
        }

        // Register the input message port and the callback function
        message_port_register_in(pmt::mp("in"));
        set_msg_handler(pmt::mp("in"), boost::bind(&ao40_encoder_impl::handler, this, _1));
    }

    ao40_encoder_impl::~ao40_encoder_impl() {}


    /*void
    ao40_encoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      // <+forecast+> e.g. ninput_items_required[0] = noutput_items
    }*/


    // Handler for incoming messages
    void ao40_encoder_impl::handler(const pmt::pmt_t msg)
    {
      // Get the length of the message
      pmt::pmt_t vector = pmt::cdr(msg);

      // Get the payload as an array
      size_t length;
      const uint8_t* payload = u8vector_elements(vector, length);

      // Check the length. Should always be 256???
      if (length != AO40_FRAME_BYTE) {
        if (debug) {
          std::cout << "ao40_encoder::handler -- invalid payload size" << std::endl;
        }
        // Do not process this frame
        return;
      }

      // Create a frame to handle the encoded data
      std::vector<uint8_t> frame;
      frame.resize(AO40_CODED_SIZE);
      encoder->ao40_encode(frame.data(), (uint8_t*)payload);

      if (debug) {
        //std::cout << "ao40_encoder::handler -- encoded [" << messages_received << ", "
        //          << frame.size() << "]" << std::endl;
      }
      messages_received++;

      // Store the modulated frame
      queue.push(frame);
    }


    int
    ao40_encoder_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      unsigned char *out = (unsigned char *) output_items[0];

      // Check to see how many messages are in the queue
      if (queue.size() > 0) {
        //std::cout << "ao40_encoder::work -- messages/waiting [" << messages_received << ", "
        //          << queue.size() << "]" << std::endl;

        std::vector<uint8_t> encoded_msg = queue.front();
        //std::cout << "ao40_encoder::work -- sizes [" << encoded_msg.size() << ", "
        //          << noutput_items << "]" << std::endl;

        // Make sure there is enough buffer space
        if (noutput_items < encoded_msg.size() + 64){
          //std::cout << "ao40_encoder::work -- buffer is too small!" << std::endl;
          return 0;
        }

        // **** THIS IS THE HARD WAY ****
        // Insert the marker - 0x1acffc1d
        /*uint64_t* flag1 = (uint64_t*)out;
        flag1[0] = 0x0001000101000000;
        flag1[1] = 0x0101010100000101;
        flag1[2] = 0x0000010101010101;
        flag1[3] = 0x0100010101000000;*/

        // This it the backwards version
        //flag1[0] = 0x0000000101000100;
        //flag1[1] = 0x0101000001010101;
        //flag1[2] = 0x0101010101010000;
        //flag1[3] = 0x0000000101010001;
        // **** </hard> ****

        // This is the easy way
        memcpy(out, marker_buf, 32);
        memcpy(out + 32, encoded_msg.data(), encoded_msg.size());
        memcpy(out + 5232, marker_buf, 32);

        /*uint64_t* flag2 = (uint64_t*)(out + 5232);
        flag2[0] = 0x0001000101000000;
        flag2[1] = 0x0101010100000101;
        flag2[2] = 0x0000010101010101;
        flag2[3] = 0x0100010101000000;*/

        // This it the backwards version
        //flag1[0] = 0x0000000101000100;
        //flag1[1] = 0x0101000001010101;
        //flag1[2] = 0x0101010101010000;
        //flag1[3] = 0x0000000101010001;

        queue.pop();
        messages_transmitted++;
        //std::cout << "ao40_encoder::work -- transmitted [" << messages_transmitted << "]"
        //          << std::endl;
        return encoded_msg.size() + 64;
      }

      // Tell the runtime how many items were produced.
      if (continuous) {
        memset(out, 0, noutput_items);
        return noutput_items;
      } else {
        return 0;
      }



    }

  } /* namespace vtgs */
} /* namespace gr */
