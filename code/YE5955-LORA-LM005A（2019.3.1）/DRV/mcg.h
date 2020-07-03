/*
 * File:    pll_init.h
 * Purpose: pll_driver specific declarations
 *
 * Notes:
 */
#ifndef __MCG_H__
#define __MCG_H__
/********************************************************************/

/* For some reason CW needs to have cw.h explicitly included here for
 * the code relocation of set_sys_dividers() to work correctly even
 * though common.h should pull in cw.h.
 */
#if (defined(CW))
	#include "cw.h"
#endif

// Constants for use in pll_init
#define NO_OSCINIT 0
#define OSCINIT 1

#define OSC_0 0
#define OSC_1 1

#define LOW_POWER 0
#define HIGH_GAIN 1

#define CANNED_OSC  0
#define CRYSTAL 1

#define PLL_0 0
#define PLL_1 1

#define PLL_ONLY 0
#define MCGOUT 1

// MCG Mode defines

#define BLPI 1
#define FBI  2
#define FEI  3
#define FEE  4
#define FBE  5
#define BLPE 6
#define PBE  7
#define PEE  8

// IRC defines
#define SLOW_IRC 0
#define FAST_IRC 1


#define LLWU_LPTMR_ME   0x01
#define LLWU_CMP0_ME   0x02
#define LLWU_CMP1_ME   0x04
#define LLWU_CMP2_ME   0x08
#define LLWU_TSI_ME   0x10
#define LLWU_RTCA_ME   0x20
#define LLWU_RESRV_ME   0x40
#define LLWU_RTCS_ME   0x80

#define LLWU_PIN_DIS 0
#define LLWU_PIN_RISING 1
#define LLWU_PIN_FALLING 2
#define LLWU_PIN_ANY 3



#define PLL0_PRDIV      4       // divide reference by 4 = 2 MHz
#define PLL0_VDIV       24      // multiply reference by 24 = 48 MHz
#define IO_NMI_DEF 1  // 1 - sets the Port interrupt, 0 - sets the NMI interrupt

#define CLK0_FREQ_HZ        8000000  //Ä¬ÈÏÎª8
#define CLK0_TYPE           CRYSTAL


unsigned char fll_rtc_init(unsigned char, unsigned char);


// prototypes
void rtc_as_refclk(void);
int fee_fei(int slow_irc_freq);
int fei_fbe(int crystal_val, unsigned char hgo_val, unsigned char erefs_val);
int fbe_fei(int slow_irc_freq);
int fei_fbi(int irc_freq, unsigned char irc_select);
int fbi_fei(int slow_irc_freq);
int fbe_pbe(int crystal_val, signed char prdiv_val, signed char vdiv_val);
int pbe_pee(int crystal_val);
int pee_pbe(int crystal_val);
int pbe_fbe(int crystal_val);
int fbe_fbi(int irc_freq, unsigned char irc_select);
int fbi_fbe(int crystal_val, unsigned char hgo_val, unsigned char erefs_val);
int fbi_fee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val);
int fbe_fee(int crystal_val);
int fee_fbe(int crystal_val);
int pbe_blpe(int crystal_val);
int blpe_pbe(int crystal_val, signed char prdiv_val, signed char vdiv_val);
int blpe_fbe(int crystal_val);
int fbi_blpi(int irc_freq, unsigned char irc_select);
int blpi_fbi(int irc_freq, unsigned char irc_select);
int fei_fee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val);
int fee_fbi(int irc_freq, unsigned char irc_select);
int fbe_blpe(int crystal_val);

int pll_init(int crystal_val, unsigned char hgo_val, unsigned char erefs_val, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select);
int new_pll_freq(int crystal_val, signed char prdiv_val, signed char vdiv_val);

int fll_freq(int fll_ref);
unsigned char what_mcg_mode(void);
unsigned char atc(unsigned char irc_select, int irc_freq, int mcg_out_freq);
void clk_monitor_0(unsigned char en_dis);
unsigned char chk_for_resistor(void);


/********************************************************************/
#endif /* __MCG_H__ */
