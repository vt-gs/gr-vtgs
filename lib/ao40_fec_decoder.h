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

#ifndef INCLUDED_VTGS_AO40_FEC_DECODER_IMPL_H
#define INCLUDED_VTGS_AO40_FEC_DECODER_IMPL_H

// Defines for RS decoders(s)
#define PNN        255
#define KK        223
#define NROOTS     32            // PNN-KK
#define FCR       112
#define PRIM       11
#define IPRIM     116
#define A00       PNN
#define BLOCKSIZE 256            // Data bytes per frame
#define RSBLOCKS    2            // Number of RS decoder blocks
#define RSPAD      95            // Unused bytes in block  (KK-BLOCKSIZE/RSBLOCKS)

// Defines for Viterbi Decoder for r=1/2 k=7  (to CCSDS convention)
#define PK 7                     // Constraint length
#define PN 2                     // Number of symbols per data bit
#define CPOLYA 0x4f              // First  convolutional encoder polynomial
#define CPOLYB 0x6d              // Second convolutional encoder polynomial

/**
 * Number of symbols in an FEC block that are
 * passed to the Viterbi decoder  (320*8 + 6)
 */
#define NBITS ((BLOCKSIZE+NROOTS*RSBLOCKS)*8+PK-1)

// Defines for Interleaver
#define ROWS       80            // Block interleaver rows
#define COLUMNS    65            // Block interleaver columns
#define SYMPBLOCK (ROWS*COLUMNS) // Encoded symbols per block

// Defines for Re-encoder
#define SYNC_POLY   0x48         // Sync vector PN polynomial


namespace gr {
  namespace vtgs {

    inline int mod255(int x);

    // AO-40 style FEC encoder
    class ao40_fec_encoder
    {
     private:
      int Nbytes;                               // Byte counter for encode_data()
      int Bindex;                               // Byte counter for interleaver
      unsigned char Conv_sr;                    // Convolutional encoder shift register state
      unsigned char RS_block[RSBLOCKS][NROOTS]; // RS parity blocks
      unsigned char encoded[SYMPBLOCK];         // Encoded symbols

      void interleave_symbol(int c);
      void encode_and_interleave(unsigned char c, int cnt);
      void scramble_and_encode(unsigned char c);
      void init_encoder();
      void encode_byte(unsigned char c);
      void encode_parity();

     public:
      ao40_fec_encoder();
      void ao40_encode(unsigned char *output, unsigned char *input);
    };

    // AO-40 style FEC decoder
    class ao40_fec_decoder
    {
     private:
      bool status;
      ao40_fec_encoder *encoder;

      void viterbi27(
        unsigned char *data,
        unsigned char *symbols,
        unsigned int nbits
      );

      int decode_rs_8(char *data, int *eras_pos, int no_eras); 


     public:
      ao40_fec_decoder();
      ~ao40_fec_decoder();
      bool get_status() const;

      int ao40_decode(
        unsigned char *output,
        unsigned char *input,
        unsigned int nblocks
      );

    };

  } // namespace vtgs
} // namespace gr

#endif /* INCLUDED_VTGS_AO40_FEC_DECODER_IMPL_H */

