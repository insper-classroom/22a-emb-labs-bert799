#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX) // esse j? est? pronto.

#define TRG_PIO				PIOD
#define TRG_PIO_ID			ID_PIOD
#define TRG_PIO_IDX			30
#define TRG_PIO_IDX_MASK	(1 << TRG_PIO_IDX)

#define ECH_PIO				PIOA
#define ECH_PIO_ID			ID_PIOA
#define ECH_PIO_IDX			6
#define ECH_PIO_IDX_MASK	(1 << ECH_PIO_IDX)

volatile char flag_rtc_alarm = 0;
volatile char flag_rtc_sec = 0;
volatile char flag_but1 = 0;
volatile char flag_echo = 0;
volatile int tempo = 0;
volatile float freq = (float) 1/(0.000058*2);

void BUT_init(Pio *pio, uint32_t ul_id, uint32_t mask, int estado);
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);

void echo_callback(void){
	if (flag_echo == 0){
		flag_echo = 1;
		RTT_init(freq, 0, 0);
		}
	else{
		flag_echo = 0;
		tempo = rtt_read_timer_value(RTT);
		}
}

void but1_callback(void){
	flag_but1 = 1;
}

void RTT_Handler(void) {
	uint32_t ul_status;

	/* Get RTT status - ACK */
	ul_status = rtt_get_status(RTT);

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		RTT_init(4, 0, RTT_MR_RTTINCIEN);
	}
	
	/* IRQ due to Time has changed */
	if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC) {

	}

}

static float get_time_rtt(){
	uint ul_previous_time = rtt_read_timer_value(RTT);
}

void draw (int time){
	char string [20];
	float dist;
	dist = ((float)time/(freq*2))*340;
	sprintf(string, "dist: %2.2f m", dist);
	gfx_mono_draw_string(string,0 ,0, &sysfont);
}

void BUT_init(Pio *pio, uint32_t ul_id, uint32_t mask, int estado) {
	pmc_enable_periph_clk(ul_id);
	pio_configure(pio, PIO_INPUT, mask, PIO_PULLUP|PIO_DEBOUNCE);
	pio_set_debounce_filter(pio, mask, 60);
};

static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

	uint16_t pllPreScale = (int) (((float) 32768) / freqPrescale);
	
	rtt_sel_source(RTT, false);
	rtt_init(RTT, pllPreScale);
	
	if (rttIRQSource & RTT_MR_ALMIEN) {
		uint32_t ul_previous_time;
		ul_previous_time = rtt_read_timer_value(RTT);
		while (ul_previous_time == rtt_read_timer_value(RTT));
		rtt_write_alarm_time(RTT, IrqNPulses+ul_previous_time);
	}

	/* config NVIC */
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 4);
	NVIC_EnableIRQ(RTT_IRQn);

	/* Enable RTT interrupt */
	if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN))
	rtt_enable_interrupt(RTT, rttIRQSource);
	else
	rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);
	
}

void io_init(void){	
	BUT_init(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, 1);
	pmc_enable_periph_clk(ECH_PIO_ID);
	pmc_enable_periph_clk(TRG_PIO_ID);
	pio_set_output(TRG_PIO, TRG_PIO_IDX_MASK, 0, 0, 0);
	pio_configure(ECH_PIO, PIO_INPUT, ECH_PIO_IDX_MASK, PIO_DEBOUNCE);
	pio_set_debounce_filter(ECH_PIO_IDX, ECH_PIO_IDX_MASK, 60);
	
	pio_handler_set(BUT1_PIO,
					BUT1_PIO_ID,
					BUT1_PIO_IDX_MASK,
					PIO_IT_EDGE,
					but1_callback);
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT1_PIO);
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 2);
	
	pio_handler_set(ECH_PIO,
					ECH_PIO_ID,
					ECH_PIO_IDX_MASK,
					PIO_IT_EDGE,
					echo_callback);
	pio_enable_interrupt(ECH_PIO, ECH_PIO_IDX_MASK);
	pio_get_interrupt_status(ECH_PIO);
	NVIC_EnableIRQ(ECH_PIO_ID);
	NVIC_SetPriority(ECH_PIO_ID, 1);
}

int main (void)
{
	sysclk_init();
	WDT->WDT_MR = WDT_MR_WDDIS;
	io_init();	
	board_init();
	delay_init();

	// Init OLED
	gfx_mono_ssd1306_init();

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		if(flag_but1){
			flag_but1 = 0;
			pio_set(TRG_PIO, TRG_PIO_IDX_MASK);
			delay_us(10);
			pio_clear(TRG_PIO, TRG_PIO_IDX_MASK);
			draw(tempo);
		}
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	}
}
