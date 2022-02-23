/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO   PIOC        
#define LED_PIO_ID    ID_PIOC    // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX      8 
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)

// Configuracoes do botao
#define BUT_PIO		PIOA
#define BUT_PIO_ID  ID_PIOA
#define BUT_PIO_IDX	11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX) // esse já está pronto.

// Configuracoes dos LEDs
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

// Configuracoes dos Botoes
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

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

void blinkLED (Pio * led, const uint32_t led_mask);
/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/



void blinkLED (Pio * led, const uint32_t led_mask){
	for (int i = 0; i < 25; i++){
		pio_clear(led, led_mask);      // Coloca 1 no pino LED da placa
		delay_ms(100);                        // Delay por software de 100 ms
		pio_set(led, led_mask);    // Coloca 0 no pino do LED
		delay_ms(100);                        // Delay por software de 100 ms
	}
}

// Função de inicialização do uC
void init(void)
{
	// Initialize the board clock
	sysclk_init();
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED.
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	
	//Config LEDs placa
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	
	// Inicializa PIO dos botoes da placa
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	
	pmc_enable_periph_clk(BUT3_PIO_ID);	
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
  while (1)
  {
	 if(!pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)) {
			blinkLED(LED_PIO, LED_PIO_IDX_MASK);
	 } 
	 
	if(!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) {
		 blinkLED(LED1_PIO, LED1_PIO_IDX_MASK);
	 }
	 
	 if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)) {
		 blinkLED(LED2_PIO, LED2_PIO_IDX_MASK);
	 }	 
	 
	 if(!pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)) {
		 blinkLED(LED3_PIO, LED3_PIO_IDX_MASK);
	 }	
	 	  
	 else {
		pio_set(LED_PIO, LED_PIO_IDX_MASK);
		
		pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
		
		pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
		
		pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
	 }
  }
  return 0;
}
