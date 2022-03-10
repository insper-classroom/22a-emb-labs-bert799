/************************************************************************
 * 5 semestre - Eng. da Computao - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Material:
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 *
 * Objetivo:
 *  - Demonstrar interrupção do PIO
 *
 * Periféricos:
 *  - PIO
 *  - PMC
 *
 * Log:
 *  - 10/2018: Criação
 ************************************************************************/

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// LED
#define LED_PIO      PIOC
#define LED_PIO_ID   ID_PIOC
#define LED_IDX      8
#define LED_IDX_MASK (1 << LED_IDX)

#define LED1_PIO	PIOA
#define LED1_PIO_ID ID_PIOA
#define LED1_PIO_IDX 0
#define LED1_PIO_IDX_MASK (1<< LED1_PIO_IDX)

#define LED2_PIO	PIOC
#define LED2_PIO_ID ID_PIOC
#define LED2_PIO_IDX 30
#define LED2_PIO_IDX_MASK (1<< LED2_PIO_IDX)

#define LED3_PIO	PIOB
#define LED3_PIO_ID ID_PIOB
#define LED3_PIO_IDX 2
#define LED3_PIO_IDX_MASK (1<< LED3_PIO_IDX)

// Botão
#define BUT_PIO      PIOA
#define BUT_PIO_ID   ID_PIOA
#define BUT_IDX  11
#define BUT_IDX_MASK (1 << BUT_IDX)

#define BUT1_PIO		PIOD
#define BUT1_PIO_ID  ID_PIOD
#define BUT1_PIO_IDX	28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX) // esse já está pronto.

#define BUT2_PIO		PIOC
#define BUT2_PIO_ID  ID_PIOC
#define BUT2_PIO_IDX	31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX) // esse já está pronto.

#define BUT3_PIO		PIOA
#define BUT3_PIO_ID  ID_PIOA
#define BUT3_PIO_IDX	19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX) // esse já está pronto.

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

volatile char but_flag; // 

volatile int frequencia;

/************************************************************************/
/* prototype                                                            */
/************************************************************************/
void io_init(void);
void pisca_led(int n, int t);

/************************************************************************/
/* handler / callbacks                                                  */
/************************************************************************/

/*
 * Exemplo de callback para o botao, sempre que acontecer
 * ira piscar o led por 5 vezes
 *
 * !! Isso é um exemplo ruim, nao deve ser feito na pratica, !!
 * !! pois nao se deve usar delays dentro de interrupcoes    !!
 */

/************************************************************************/
/* funções                                                              */
/************************************************************************/

// pisca led N vez no periodo T
void pisca_led(int n, int t){
  for (int i=0;i<n;i++){
    pio_clear(LED_PIO, LED_IDX_MASK);
    delay_ms(t);
    pio_set(LED_PIO, LED_IDX_MASK);
    delay_ms(t);
  }
}


void draw_frequency(frequencia){
	char freq_str[20];
	sprintf(freq_str, "%d ms", frequencia);
	gfx_mono_draw_string(freq_str, 5,16, &sysfont);
}

void but_callback(void){
	but_flag = 1;
}
void but_callback(void){
	but_flag = 1;
}
void but_callback(void){
	but_flag = 1;
}
void but_callback(void){
	but_flag = 1;
}

// Inicializa botao SW0 do kit com interrupcao
void io_init(void)
{

  // Configura led
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_IDX_MASK, PIO_DEFAULT);

	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
	
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED3_PIO, PIO_OUTPUT_0, LED3_PIO_IDX_MASK, PIO_DEFAULT);

  // Inicializa clock do periférico PIO responsavel pelo botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);

  // Configura PIO para lidar com o pino do botão como entrada
  // com pull-up
	pio_configure(BUT_PIO, PIO_INPUT, BUT_IDX_MASK, PIO_PULLUP|PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT_IDX_MASK, 60);
	
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP|PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP|PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT2_PIO_IDX_MASK, 60);
	
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP|PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT3_PIO_IDX_MASK, 60);
  // Configura interrupção no pino referente ao botao e associa
  // função de callback caso uma interrupção for gerada
  // a função de callback é a: but_callback()
  pio_handler_set(BUT_PIO,
                  BUT_PIO_ID,
                  BUT_IDX_MASK,
                  PIO_IT_RISE_EDGE,
                  but_callback);
				  

 pio_handler_set(BUT1_PIO,
                  BUT1_PIO_ID,
                  BUT1_PIO_IDX_MASK,
                  PIO_IT_RISE_EDGE,
                  but_callback);

 pio_handler_set(BUT2_PIO,
                  BUT2_PIO_ID,
                  BUT2_PIO_IDX_MASK,
                  PIO_IT_RISE_EDGE,
                  but_callback);
				  
 pio_handler_set(BUT3_PIO,
                  BUT3_PIO_ID,
                  BUT3_PIO_IDX_MASK,
                  PIO_IT_RISE_EDGE,
                  but_callback);
				  
  // Ativa interrupção e limpa primeira IRQ gerada na ativacao
  pio_enable_interrupt(BUT_PIO, BUT_IDX_MASK);
  pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
  pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
  pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
  pio_get_interrupt_status(BUT_PIO);
  pio_get_interrupt_status(BUT1_PIO);
  pio_get_interrupt_status(BUT2_PIO);
  pio_get_interrupt_status(BUT3_PIO);
  
  // Configura NVIC para receber interrupcoes do PIO do botao
  // com prioridade 4 (quanto mais próximo de 0 maior)
  NVIC_EnableIRQ(BUT_PIO_ID);
  NVIC_EnableIRQ(BUT1_PIO_ID);
  NVIC_EnableIRQ(BUT2_PIO_ID);
  NVIC_EnableIRQ(BUT3_PIO_ID);
  NVIC_SetPriority(BUT_PIO_ID, 4); // Prioridade 4
  NVIC_SetPriority(BUT1_PIO_ID, 3);
  NVIC_SetPriority(BUT2_PIO_ID, 2);
  NVIC_SetPriority(BUT3_PIO_ID, 1); 
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
void main(void)
{
	// Inicializa clock
	sysclk_init();

	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

  // configura botao com interrupcao
  io_init();

	// super loop
	// aplicacoes embarcadas no devem sair do while(1).
	
	while(1)
  {
	     if (but_flag) {  //
		     pisca_led(5, 200);
		     but_flag = 0;  //
		}
		gfx_mono_draw_string("             ", 0,10, &sysfont);
		gfx_mono_draw_string("Star Wars", 0,10, &sysfont);
		// Entra em sleep mode
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); //
	}
}
