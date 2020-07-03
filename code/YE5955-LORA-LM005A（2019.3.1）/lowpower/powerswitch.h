#include <stdint.h>
#include "includes.h"


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

#define CLK0_FREQ_HZ        16000000  //Ä¬ÈÏÎª8
#define CLK0_TYPE           CRYSTAL

int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int periph_clk_khz;
int pll_clk_khz;
int uart0_clk_khz;
uint32 uart0_clk_hz;
extern uint32_t SystemCoreClock;

void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en );
void LLWU_Init(void);
unsigned char what_mcg_mode(void);
void vlp_clock_config(char next_mode);
int enter_vlpr(void);
void enter_vlps(void);
void clockMonitor(unsigned char state);
void set_4_lp(unsigned char outof_lp);
