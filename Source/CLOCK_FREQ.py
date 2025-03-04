#!/usr/bin/env python3
#------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------
#
# Script used to find suitable RPiPico PLL SYS configs 

def pll(refdiv_6, fbdiv_12, post_div1_3, post_div2_3, clk_min, clk_max):
   """ Compute PLL frequency """

   XTAL    =   12000000
   VCO_MIN =  756000000
   VCO_MAX = 1596000000

   ref_freq = XTAL / refdiv_6
   vco_freq = ref_freq * fbdiv_12

   if vco_freq < VCO_MIN or vco_freq > VCO_MAX:
      return False, None, vco_freq

   pll_freq = vco_freq / (post_div1_3 * post_div2_3)

   if pll_freq < clk_min or pll_freq > clk_max:
      return False, pll_freq, vco_freq

   return True, pll_freq, vco_freq


def enumeratePllClockConfigs(clk_min_mhz, clk_max_mhz):
   """ Find all possible configs for a range of target clocks """

   table = {}

   for refdiv in range(1, 32):
      for fbdiv in range(1, 4096):
         for post_div1 in range(7, 0, -1):
            for post_div2 in range(1, 8):
               ok, pll_freq, vco_freq = pll(refdiv, fbdiv, post_div1, post_div2,
                                            clk_min_mhz * 1000000, clk_max_mhz * 1000000)
               if ok:
                  record = { "refdiv"    : refdiv,
                             "fbdiv"     : fbdiv,
                             "post_div1" : post_div1,
                             "post_div2" : post_div2,
                             "vco_freq"  : vco_freq }
                  if pll_freq not in table:
                     table[pll_freq] = record

                  elif record['vco_freq'] > table[pll_freq]['vco_freq']:
                     table[pll_freq] = record

   return dict(sorted(table.items()))


table = enumeratePllClockConfigs(clk_min_mhz = 132, clk_max_mhz = 420)

# For found PLL clock configs filter for those that are exact mutliples
# of frequencies near to the target sample rate of 49096 Hz

target_freq    = 49096
max_freq_error = 4

for freq in table:
   record = table[freq]
   for f in range(target_freq - max_freq_error, target_freq + max_freq_error + 1):
      if int(freq) % f == 0:
         print(f'{f} Hz from clock {freq:.3f} Hz with config {record}')
