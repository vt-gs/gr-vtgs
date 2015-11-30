#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Default Capture
# Generated: Mon Nov 30 14:10:29 2015
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import time

class default_capture(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Default Capture")

        ##################################################
        # Variables
        ##################################################
        self.uplink_offset = uplink_offset = 0
        self.uplink_freq = uplink_freq = 145.95e6
        self.samp_rate = samp_rate = 250e3
        self.downlink_offset = downlink_offset = 0
        self.downlink_freq = downlink_freq = 435.85e6

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(downlink_freq, 0)
        self.uhd_usrp_source_0.set_gain(0, 0)
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(1, (), downlink_offset, samp_rate)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/tmp/test.dat", False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))    


    def get_uplink_offset(self):
        return self.uplink_offset

    def set_uplink_offset(self, uplink_offset):
        self.uplink_offset = uplink_offset

    def get_uplink_freq(self):
        return self.uplink_freq

    def set_uplink_freq(self, uplink_freq):
        self.uplink_freq = uplink_freq

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def get_downlink_offset(self):
        return self.downlink_offset

    def set_downlink_offset(self, downlink_offset):
        self.downlink_offset = downlink_offset
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.downlink_offset)

    def get_downlink_freq(self):
        return self.downlink_freq

    def set_downlink_freq(self, downlink_freq):
        self.downlink_freq = downlink_freq
        self.uhd_usrp_source_0.set_center_freq(self.downlink_freq, 0)


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = default_capture()
    tb.start()
    tb.wait()
