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

#include "ao40_fec_decoder.h"
#include "ao40_fec_tables.h"

#include <memory>
#include <cstring>

namespace gr {
  namespace vtgs {

    int mod255(int x)
    {
        while (x >= 255) {
            x -= 255;
            x = (x >> 8) + (x & 255);
        }
        return x;
    }

    void ao40_fec_decoder::viterbi27(
        unsigned char *data, 
        unsigned char *symbols, 
        unsigned int nbits)
    {
        unsigned int bitcnt = 0;
        int beststate, i, j;
        long cmetric[64], nmetric[64]; // 2^(K-1) 
        unsigned long *pp;
        long m0, m1, mask;
        int mets[4]; // 2^N
        unsigned long *paths;

        pp = paths = new unsigned long[nbits * 2](); 
     
        // Initialize starting metrics to prefer 0 state 
        cmetric[0] = 0;

        for (i = 1; i < 64; i++)
            cmetric[i] = -999999;

        for (;;) {
            // Read 2 input symbols and compute the 4 branch metrics 
            for (i = 0; i < 4; i++) {
                mets[i] = 0;
                for (j = 0; j < 2; j++) {
                    mets[i] += mettab[(i >> (1 - j)) & 1][symbols[j]];
                }
            } 

            symbols += 2;
            mask = 1;

            for (i = 0; i < 64; i += 2) {
                int b1, b2;

                b1 = mets[Syms[i]];
                nmetric[i] = m0 = cmetric[i / 2] + b1;
                b2 = mets[Syms[i + 1]];
                b1 -= b2;
                m1 = cmetric[(i / 2) + (1 << (PK - 2))] + b2;

                if (m1 > m0) {
                    nmetric[i] = m1;
                    *pp |= mask;
                }

                m0 -= b1;
                nmetric[i + 1] = m0;
                m1 += b1;

                if (m1 > m0) {
                    nmetric[i + 1] = m1;
                    *pp |= mask << 1;
                }

                mask <<= 2; // assumes sizeof(long)==32bits? hmmn.

                if ((mask & 0xffffffff) == 0) {
                    mask = 1;
                    pp++;
                }
            }

            if (mask != 1) { 
                pp++;
            }

            if (++bitcnt == nbits) {
                beststate = 0;
                break;
            }

            std::memcpy(cmetric, nmetric, sizeof(cmetric));
        }

        pp -= 2;

        // Chain back from terminal state to produce decoded data 
        std::memset(data, 0, nbits / 8);
        for (i = nbits - PK; i >= 0; i--) {
            if (pp[beststate >> 5] & (1L << (beststate & 31))) {
                beststate |= (1 << (PK - 1));
                data[i >> 3] |= 0x80 >> (i & 7);
            }

            beststate >>= 1;
            pp -= 2;
        }

        delete [] paths;
    }

    int ao40_fec_decoder::decode_rs_8(char *data, int *eras_pos, int no_eras)
    {
        int deg_lambda, el, deg_omega;
        int i, j, r, k;
        unsigned char u, q, tmp, num1, num2, den, discr_r;
        unsigned char lambda[NROOTS + 1], s[NROOTS];    // Err+Eras Locator poly and syndrome poly 
        unsigned char b[NROOTS + 1], t[NROOTS + 1], omega[NROOTS + 1];
        unsigned char root[NROOTS], reg[NROOTS + 1], loc[NROOTS];
        int syn_error, count;

        // form the syndromes; i.e., evaluate data(x) at roots of g(x) 
        for (i = 0; i < NROOTS; i++)
            s[i] = data[0];

        for (j = 1; j < PNN; j++) {
            for (i = 0; i < NROOTS; i++) {
                if (s[i] == 0) {
                    s[i] = data[j];
                } else {
                    s[i] =
                        data[j] ^
                        ALPHA_TO[mod255
                                 (INDEX_OF[s[i]] + (FCR + i) * PRIM)];
                }
            }
        }

        // Convert syndromes to index form, checking for nonzero condition 
        syn_error = 0;
        for (i = 0; i < NROOTS; i++) {
            syn_error |= s[i];
            s[i] = INDEX_OF[s[i]];
        }

        if (!syn_error) {
            // if syndrome is zero, data[] is a codeword and there are no
            // errors to correct. So return data[] unmodified
            count = 0;
            goto finish;
        }
        std::memset(&lambda[1], 0, NROOTS * sizeof(lambda[0]));
        lambda[0] = 1;

        if (no_eras > 0) {
            // Init lambda to be the erasure locator polynomial 

            lambda[1] = ALPHA_TO[mod255(PRIM * (PNN - 1 - eras_pos[0]))];
            for (i = 1; i < no_eras; i++) {
                u = mod255(PRIM * (PNN - 1 - eras_pos[i]));
                for (j = i + 1; j > 0; j--) {
                    tmp = INDEX_OF[lambda[j - 1]];
                    if (tmp != A00)
                        lambda[j] ^= ALPHA_TO[mod255(u + tmp)];
                }
            }
        }
        for (i = 0; i < NROOTS + 1; i++)
            b[i] = INDEX_OF[lambda[i]];

        // Begin Berlekamp-Massey algorithm to determine error+erasure
        // locator polynomial
        r = no_eras;
        el = no_eras;
        while (++r <= NROOTS) { // r is the step number 
            // Compute discrepancy at the r-th step in poly-form 
            discr_r = 0;
            for (i = 0; i < r; i++) {
                if ((lambda[i] != 0) && (s[r - i - 1] != A00)) {
                    discr_r ^=
                        ALPHA_TO[mod255
                                 (INDEX_OF[lambda[i]] + s[r - i - 1])];
                }
            }
            discr_r = INDEX_OF[discr_r];        // Index form
            if (discr_r == A00) {
                // 2 lines below: B(x) <-- x*B(x) 
                std::memmove(&b[1], b, NROOTS * sizeof(b[0]));
                b[0] = A00;
            } else {
                // 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x)
                t[0] = lambda[0];
                for (i = 0; i < NROOTS; i++) {
                    if (b[i] != A00)
                        t[i + 1] =
                            lambda[i +
                                   1] ^ ALPHA_TO[mod255(discr_r + b[i])];
                    else
                        t[i + 1] = lambda[i + 1];
                }
                if (2 * el <= r + no_eras - 1) {
                    el = r + no_eras - el;
                    // 2 lines below: B(x) <-- inv(discr_r) *
                    // lambda(x)
                    for (i = 0; i <= NROOTS; i++)
                        b[i] =
                            (lambda[i] ==
                             0) ? A00 : mod255(INDEX_OF[lambda[i]] -
                                              discr_r + PNN);
                } else {
                    // 2 lines below: B(x) <-- x*B(x)
                    std::memmove(&b[1], b, NROOTS * sizeof(b[0]));
                    b[0] = A00;
                }
                std::memcpy(lambda, t, (NROOTS + 1) * sizeof(t[0]));
            }
        }

        // Convert lambda to index form and compute deg(lambda(x)) 
        deg_lambda = 0;
        for (i = 0; i < NROOTS + 1; i++) {
            lambda[i] = INDEX_OF[lambda[i]];
            if (lambda[i] != A00)
                deg_lambda = i;
        }
        // Find roots of the error+erasure locator polynomial by Chien search 
        std::memcpy(&reg[1], &lambda[1], NROOTS * sizeof(reg[0]));
        count = 0;              /* Number of roots of lambda(x) */
        for (i = 1, k = IPRIM - 1; i <= PNN; i++, k = mod255(k + IPRIM)) {
            q = 1;              /* lambda[0] is always 0 */
            for (j = deg_lambda; j > 0; j--) {
                if (reg[j] != A00) {
                    reg[j] = mod255(reg[j] + j);
                    q ^= ALPHA_TO[reg[j]];
                }
            }
            if (q != 0)
                continue;       /* Not a root */
            /* store root (index-form) and error location number */
            root[count] = i;
            loc[count] = k;
            /* If we've already found max possible roots,
             * abort the search to save time
             */
            if (++count == deg_lambda)
                break;
        }
        if (deg_lambda != count) {
            /*
             * deg(lambda) unequal to number of roots => uncorrectable
             * error detected
             */
            count = -1;
            goto finish;
        }
        /*
         * Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo
         * x**NROOTS). in index form. Also find deg(omega).
         */
        deg_omega = 0;
        for (i = 0; i < NROOTS; i++) {
            tmp = 0;
            j = (deg_lambda < i) ? deg_lambda : i;
            for (; j >= 0; j--) {
                if ((s[i - j] != A00) && (lambda[j] != A00))
                    tmp ^= ALPHA_TO[mod255(s[i - j] + lambda[j])];
            }
            if (tmp != 0)
                deg_omega = i;
            omega[i] = INDEX_OF[tmp];
        }
        omega[NROOTS] = A00;

        /*
         * Compute error values in poly-form. num1 = omega(inv(X(l))), num2 =
         * inv(X(l))**(FCR-1) and den = lambda_pr(inv(X(l))) all in poly-form
         */
        for (j = count - 1; j >= 0; j--) {
            num1 = 0;
            for (i = deg_omega; i >= 0; i--) {
                if (omega[i] != A00)
                    num1 ^= ALPHA_TO[mod255(omega[i] + i * root[j])];
            }
            num2 = ALPHA_TO[mod255(root[j] * (FCR - 1) + PNN)];
            den = 0;

            /* lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i] */
            for (i = std::min(deg_lambda, NROOTS - 1) & ~1; i >= 0; i -= 2) {
                if (lambda[i + 1] != A00)
                    den ^= ALPHA_TO[mod255(lambda[i + 1] + i * root[j])];
            }
            if (den == 0) {
                count = -1;
                goto finish;
            }
            /* Apply error to data */
            if (num1 != 0) {
                data[loc[j]] ^=
                    ALPHA_TO[mod255
                             (INDEX_OF[num1] + INDEX_OF[num2] + PNN -
                              INDEX_OF[den])];
            }
        }

        finish:

        if (eras_pos != NULL) {
            for (i = 0; i < count; i++)
                eras_pos[i] = loc[i];
        }
        return count;
    } 

    ao40_fec_encoder::ao40_fec_encoder()
    {
    }

    void ao40_fec_encoder::interleave_symbol(int c)
    {
        int row, col;
        col = Bindex / COLUMNS;
        row = Bindex % COLUMNS;
        if (c)
            encoded[row * ROWS + col] = 1;
        Bindex++;
    }

    void ao40_fec_encoder::encode_and_interleave(unsigned char c, int cnt)
    {
        while (cnt-- != 0) {
            Conv_sr = (Conv_sr << 1) | (c >> 7);
            c <<= 1;
            interleave_symbol(Partab[Conv_sr & CPOLYA]);
            interleave_symbol(!Partab[Conv_sr & CPOLYB]);       /* Second encoder symbol is inverted */
        }
    }

    void ao40_fec_encoder::scramble_and_encode(unsigned char c) 
    {
        c ^= Scrambler[Nbytes]; /* Scramble byte */
        encode_and_interleave(c, 8);    /* RS encode and place into encoded buffer */
    }

    void ao40_fec_encoder::init_encoder() 
    {
        int i, j, sr;

        Nbytes = 0;
        Conv_sr = 0;
        Bindex = COLUMNS;       /* Sync vector is in first column; data starts here */

        for (j = 0; j < RSBLOCKS; j++)  /* Flush parity array */
            for (i = 0; i < NROOTS; i++)
                RS_block[j][i] = 0;

        /* Clear re-encoded array */
        for (i = 0; i < 5200; i++)
            encoded[i] = 0;

        /* Generate sync vector, interleave into re-encode array, 1st column */
        sr = 0x7f;
        for (i = 0; i < 65; i++) {
            if (sr & 64)
                encoded[ROWS * i] = 1; /* Every 80th symbol is a sync bit */
            sr = (sr << 1) | Partab[sr & SYNC_POLY];
        }
    }

    void ao40_fec_encoder::encode_byte(unsigned char c)
    {

        int i;

        unsigned char *rp;      /* RS block pointer */
        unsigned char feedback;

        /* Update the appropriate Reed-Solomon codeword */
        rp = RS_block[Nbytes & 1];

        /* Compute feedback term */
        feedback = INDEX_OF[c ^ rp[0]];

        /* If feedback is non-zero, multiply by each generator polynomial coefficient and
         * add to corresponding shift register elements
         */
        if (feedback != A00) {
            int j;

            /* This loop exploits the palindromic nature of the generator polynomial
             * to halve the number of discrete multiplications
             */
            for (j = 0; j < 15; j++) {
                unsigned char t;

                t = ALPHA_TO[mod255(feedback + RS_poly[j])];
                rp[j + 1] ^= t;
                rp[31 - j] ^= t;
            }
            rp[16] ^= ALPHA_TO[mod255(feedback + RS_poly[15])];
        }

        /* Shift 32 byte RS register one position down */
        for (i = 0; i < 31; i++)
            rp[i] = rp[i + 1];

        /* Handle highest order coefficient, which is unity */
        if (feedback != A00) {
            rp[31] = ALPHA_TO[feedback];
        } else {
            rp[31] = 0;
        }

        scramble_and_encode(c);
        Nbytes++;
    }

    void ao40_fec_encoder::encode_parity() {
        unsigned char c;

        c = RS_block[Nbytes & 1][(Nbytes - 256) >> 1];
        scramble_and_encode(c);
        if (++Nbytes == 320) {
            /* Tail off the convolutional encoder (flush) */
            encode_and_interleave(0, 6);
        }
    }

    void ao40_fec_encoder::ao40_encode(unsigned char *output, unsigned char *input)   
    {
        init_encoder();

        for (size_t i = 0; i < 256; i++) 
            encode_byte(input[i]);

        for (size_t i = 0; i < 64; i++) 
            encode_parity();

        for (size_t i = 0; i < SYMPBLOCK; i++) 
            output[i] = encoded[i]; 
    }

    ao40_fec_decoder::ao40_fec_decoder()
    {
        status = false;
        encoder = new ao40_fec_encoder();
    }

    ao40_fec_decoder::~ao40_fec_decoder()
    {
        delete encoder;
    }

    int ao40_fec_decoder::ao40_decode(unsigned char *output, 
                                      unsigned char *input,  
                                      unsigned int nblocks) 
    {
        unsigned char symbols[NBITS*2 + 65 + 3];
        unsigned char vitdecdata[(NBITS - 6)/8]; 
        unsigned char encoded[SYMPBLOCK]; 

        char rsblocks[RSBLOCKS][PNN]; 

        int col, row, di, si; 
        int coltop, rowstart;
        int rs_failures; 
        int rserrors[RSBLOCKS]; 

        int errors = 0; 

        for (size_t i = 0; i < nblocks; i++) { 

            /* De-interleaver */ 
            coltop = 0;

            for (col = 1; col < ROWS; col++) {
                rowstart = 0; 
                for (row = 0; row < COLUMNS; row++) { 
                    symbols[coltop + row] = input[i*SYMPBLOCK + (rowstart + col)]; 
                    rowstart += ROWS; 
                }
                coltop += COLUMNS; 
            }

            /* Viterbi decoder */ 
            this->viterbi27(vitdecdata, symbols, NBITS); 

            std::memset(rsblocks, 0, sizeof(rsblocks)); /* Zero rsblocks array */  

            di = 0;
            si = 0; 

            for (col = RSPAD; col < PNN; col++) {
                for (row = 0; row < RSBLOCKS; row++) {
                    rsblocks[row][col] = vitdecdata[di++] ^ Scrambler[si++]; /* Remove scrambling */      
                }
            }

            /* Run RS-decoder(s) */ 
            rs_failures = 0; 
            for (row = 0; row < RSBLOCKS; row++) {
                rserrors[row] = this->decode_rs_8(rsblocks[row], NULL, 0); 
                rs_failures += (rserrors[row] == -1); 
            }

            /* If frame decoded OK, deinterleave data from RS codeword(s) */ 
            if (!rs_failures) { 
                int j = 0; 

                for (col = RSPAD; col < KK; col++) {
                    for (row = 0; row < RSBLOCKS; row++) {
                        output[i * BLOCKSIZE + j++] = rsblocks[row][col]; 
                    }
                }
                status = true;
            } else {
		// Try to still pull the data out.
		// Even though the frame decode failed, there is a chance things can be recovered?
                int j = 0; 

                for (col = RSPAD; col < KK; col++) {
                    for (row = 0; row < RSBLOCKS; row++) {
                        output[i * BLOCKSIZE + j++] = rsblocks[row][col]; 
                    }
                }
                status = false;
            }

            /* Re-encode o/p and count channel errors */
           encoder->ao40_encode(encoded, output);

            /* Count the channel errors */
            for (size_t j = 0; j < SYMPBLOCK; j++) {
                if (encoded[j] != (input[j] >> 7)) {
                    errors++;
                }
            }
        }

        return errors;
    }

    bool ao40_fec_decoder::get_status() const 
    {
        return status; 
    }

  } /* namespace vtgs */
} /* namespace gr */
