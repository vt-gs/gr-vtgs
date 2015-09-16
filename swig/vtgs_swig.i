/* -*- c++ -*- */

#define VTGS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "vtgs_swig_doc.i"

%{
#include "vtgs/afskmod.h"
#include "vtgs/afskdemod.h"
#include "vtgs/ax25_framer.h"
#include "vtgs/ax25_deframer.h"
#include "vtgs/ao40_decoder.h"
#include "vtgs/fsk96demod.h"
#include "vtgs/mult_scrambler.h"
#include "vtgs/mult_descrambler.h"
#include "vtgs/fsk96mod.h"
%}


%include "vtgs/afskmod.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, afskmod);
%include "vtgs/ax25_framer.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, ax25_framer);
%include "vtgs/ax25_deframer.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, ax25_deframer);
%include "vtgs/ao40_decoder.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, ao40_decoder);
%include "vtgs/afskdemod.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, afskdemod);
%include "vtgs/fsk96demod.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, fsk96demod);
%include "vtgs/mult_scrambler.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, mult_scrambler);
%include "vtgs/mult_descrambler.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, mult_descrambler);

%include "vtgs/fsk96mod.h"
GR_SWIG_BLOCK_MAGIC2(vtgs, fsk96mod);
