#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Default Test
# Generated: Thu Dec 10 22:45:58 2015
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser


class default_test(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Default Test")

        ##################################################
        # Variables
        ##################################################
        self.uplink_offset = uplink_offset = 0
        self.uplink_freq = uplink_freq = 400e6
        self.samp_rate = samp_rate = 250e3
        self.downlink_offset = downlink_offset = 0
        self.downlink_freq = downlink_freq = 400e6

        ##################################################
        # Blocks
        ##################################################
        self.uplink_filter = filter.freq_xlating_fir_filter_ccc(1, (1, ), uplink_freq, samp_rate)
        self.downlink_filter = filter.freq_xlating_fir_filter_ccc(1, (1, ), downlink_freq, samp_rate)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/tmp/test.dat", False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 1000, 1, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_0, 0), (self.downlink_filter, 0))    
        self.connect((self.analog_sig_source_x_0, 0), (self.uplink_filter, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_null_sink_0, 0))    
        self.connect((self.downlink_filter, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.uplink_filter, 0), (self.blocks_throttle_0_0, 0))    

    def get_uplink_offset(self):
        return self.uplink_offset

    def set_uplink_offset(self, uplink_offset):
        self.uplink_offset = uplink_offset

    def get_uplink_freq(self):
        return self.uplink_freq

    def set_uplink_freq(self, uplink_freq):
        self.uplink_freq = uplink_freq
        self.uplink_filter.set_center_freq(self.uplink_freq)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)

    def get_downlink_offset(self):
        return self.downlink_offset

    def set_downlink_offset(self, downlink_offset):
        self.downlink_offset = downlink_offset

    def get_downlink_freq(self):
        return self.downlink_freq

    def set_downlink_freq(self, downlink_freq):
        self.downlink_freq = downlink_freq
        self.downlink_filter.set_center_freq(self.downlink_freq)


def main(top_block_cls=default_test, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
