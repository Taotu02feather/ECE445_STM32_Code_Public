/*****************************************************************************
* | File      	:   hm01b0_init.h
* | Author      :   
* | Function    :   Initialize HM01B0 register
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-01-09
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
******************************************************************************/
#ifndef HM01B0_H
#define HM01B0_H
#include "sys.h"
//#include "cam.h"
//#include <stdint.h>
struct senosr_reg{
	u16 reg;
	u8  val;
};
struct senosr_reg hm01b0_324x244[] = {
    {0x0103, 0x00}, // reset 0
    {0x0100,0x00},  //standby mode
    {0x1003,0x08},   
    {0x1007,0x08}, 
    {0x3044,0x0A},     
    {0x3045,0x00},    
    {0x3047,0x0A},    
    {0x3050,0xC0},    
    {0x3051,0x42}, 
    {0x3052,0x50},
    {0x3053,0x00},
    {0x3054,0x03}, 
    {0x3055,0xF7},
    {0x3056,0xF8},
    {0x3057,0x29},
    {0x3058,0x1F},
    {0x3059,0x1E},
    {0x3064,0x00},
    {0x3065,0x04},
    {0x1000,0x43},
    {0x1001,0x40},
    {0x1002,0x32}, 
    {0x0350,0x7F},
    {0x1006,0x01},
    {0x1008,0x03},
    {0x1009,0xA0},
    {0x100A,0x60},
    {0x100B,0x10}, 
    {0x100C,0x40},
    {0x3022,0x01},
    {0x1012,0x01},
    {0x2000,0x07},
    {0x2003,0x00}, 
    {0x2004,0x1C},
    {0x2007,0x00}, 
    {0x2008,0x58}, 
    {0x200B,0x00}, 
    {0x200C,0x7A}, 
    {0x200F,0x00},
    {0x2010,0xB8},
    {0x2013,0x00},
    {0x2014,0x58},
    {0x2017,0x00},
    {0x2018,0x9B},
    {0x2100,0x01},
    {0x2101,0x5F},
    {0x2102,0x0A},
    {0x2103,0x03},
    {0x2104,0x05},
    {0x2105,0x02},
    {0x2106,0x14},
    {0x2107,0x02},
    {0x2108,0x03},
    {0x2109,0x03},
    {0x210A,0x00},
    {0x210B,0x80},
    {0x210C,0x40},
    {0x210D,0x20},
    {0x210E,0x03},
    {0x210F,0x00},
    {0x2110,0x85},
    {0x2111,0x00},
    {0x2112,0xA0},
    {0x2150,0x03},
    {0x0340,0x01},
    {0x0341,0x7A},
    {0x0342,0x01},
    {0x0343,0x77},
    {0x3010,0x00},  
    {0x0383,0x01},
    {0x0387,0x01},
    {0x0390,0x00},
    {0x3011,0x70},
    {0x3059,0x22},
    {0x3060,0x30},
    {0x0101,0x01},
    {0x0104,0x01},
    {0x0100,0x01},
    {0xFFFF,0xFF},
};
u8 hm01b0_Init(void);
u8 Clock_mco_init(void);
#endif 