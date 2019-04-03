#include <msp430g2553.h>


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	

    P1DIR|= BIT0 + BIT6;       //configure Port1-->pin0(Red_Led) and Port1-->pin6(Green_Led) as Outputs
    P1OUT&= ~(BIT0+BIT6);      //Clear any output voltage at pin0(Red_Led) and pin6(Green_Led)

    P1DIR&=~BIT3;              //configure Port1-->pin3 as Input
    P1REN|=BIT3;			   //Enable Pull_up/Pull_down register for Port1-->Pin3
    P1OUT|=BIT3;			   //Select Pull_up mode for Port1-->Pin3

    P1IE|=BIT3;                //Enable interrupt on Port1-->Pin3
    P1IFG&=~BIT3;			   //Clear interrupt flag on Port1-->Pin3
    P1IES|=BIT3;			   //Set Interrupt Edge Select on Port1-->Pin3

    __bis_SR_register(GIE);	   //Enable Global Interrupt Enable bit in Status register

    while(1);				   //Wait for interrupt to occur
}

#pragma vector=PORT1_VECTOR	   //Specify interrupt vector for Port1
__interrupt void GPIO(void)    //This is the interrupt service routine
{

	P1IFG&=~BIT3;			   //Clear the interrupt flag(no interrupt pending)
	P1IES^=BIT3;               //Toggle Edge Select bit to prevent bounce effects

	P1OUT^= BIT0+BIT6;		   //Toggle LEDs

}

