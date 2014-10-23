

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stm32f0xx.h>

int main(void)
{
  const int transmit_pin = GPIO_Pin_6;
	uint8_t count = 1;
	unsigned long i;
	vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(1); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
	
  while(1)
  {  
    char msg[1] = {'#'};

		msg[0] = count;
		vw_send((uint8_t *)msg, 1);
		vw_wait_tx(); // Wait until the whole message is gone
		for(i=0;i<4000000UL;i++);
		count = count + 1;
  }
}
