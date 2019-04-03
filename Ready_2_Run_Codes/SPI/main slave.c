#include <msp430.h>
unsigned char RxData;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  while (!(P1IN & BIT4));                   // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode


  P1SEL = BIT1 + BIT2 + BIT4;
  P1SEL2 = BIT1 + BIT2 + BIT4;
  UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
  UCA0CTL0 |= UCCKPL + UCMSB + UCSYNC;      // 3-pin, 8-bit SPI master
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt
  P1DIR |= BIT0+BIT6;
  P1OUT &= ~(BIT0+BIT6);

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}



#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void)
{
  while (!(IFG2 & UCA0RXIFG));              // USCI_A0 RX buffer ready?
  RxData = UCA0RXBUF;
  	  if(RxData&0x02)
  		P1OUT^=BIT0;
  	  else
  		  P1OUT^=BIT6;
}
