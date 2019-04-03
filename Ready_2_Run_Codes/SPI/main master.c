#include <msp430.h>
unsigned char MST_Data=0x02;

int main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1SEL = BIT1 + BIT2 + BIT4;
  P1SEL2 = BIT1 + BIT2 + BIT4;
  UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x02;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  P1DIR &= ~BIT3;
  P1REN |= BIT3;
  P1IE |= BIT3;
  P1IFG = 0;
  P1IES |= BIT3;

  P1DIR |= BIT6;
  P1OUT &= ~BIT6;
  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}

#pragma vector = PORT1_VECTOR
__interrupt void port1(void)
{
	P1IES^=BIT3;
	P1IFG&=~BIT3;
	P1OUT^=BIT6;
		  IE2 |= UCA0TXIE;
		      __bis_SR_register(LPM0_bits | GIE);     // CPU off, enable interrupts

}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void)
{
	P1OUT|=BIT0;
	UCA0TXBUF = MST_Data;                   // Transmit characters
	IE2 &= ~UCA0TXIE;
	      __bic_SR_register_on_exit(LPM0_bits); // Wake up to setup next TX
}
