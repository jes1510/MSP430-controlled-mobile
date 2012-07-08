//***************************************************************************************
//Read IR data on p1.3 and toggle motor on p1.0
//
// Toggles the motor on and off by detecting IR data on the interrupt pin p1.3. The data
// is not decoded and it triggers an ISR on each press.  The button is debounced
// in software to keep the motor from bouncing on and off.
//  
// Build with msp430-gcc
//
//
// MSP430G2231
// -----------------
//
// | button      | -->  p1.3
// | Sensor Power| -->  p1.2
// | P1.0        | -->  LED and motor driver
// 
//
//  Jesse Merritt
//  WimpyLabs
//  October 2011
//
// Based on original code by:
// Aldo Briano
// Texas Instruments, Inc
//***************************************************************************************/

//*************************************************************************************
//﻿  LIBRARY IMPORTS
//*************************************************************************************
#include <signal.h>﻿  ﻿  // Needed for ISR
#include <msp430g2211.h>

//*************************************************************************************
//﻿  GLOBAL CONSTANTS
//*************************************************************************************
#define MOTOR BIT0
#define SENSOR BIT1
#define TICKER BIT2
#define BUTTON BIT3
#define RUN_TIME 300

//*************************************************************************************
//﻿  GLOBAL VARIABLES
//*************************************************************************************
volatile unsigned int seconds = 0;
volatile unsigned int ticks = 0;
volatile unsigned int debounce_timer = 0;
volatile unsigned int debounce_diff = 0;
volatile unsigned int buttonFlag = 0;

//*************************************************************************************
//﻿  PROTOTYPES
//*************************************************************************************
static void motorOff();﻿  ﻿  ﻿  // Turns off motor
static void motorOn();﻿  ﻿  ﻿  // turns on motor


//*************************************************************************************
//﻿  function:  PORT1_ISR
//﻿  returns:  Nothing.  Detect button presses and wakes up into active mode.  
//﻿  ﻿    Sets the button flag and starts deboucne_timer fresh
//﻿  ﻿   
//*************************************************************************************
interrupt (PORT1_VECTOR) PORT1_ISR(void) {  
  P1IFG = 0x00;  ﻿  ﻿  ﻿  // Reset the PORT1 interrupt flag   
  buttonFlag = 1; ﻿  ﻿  ﻿  // Set the interrupt flag 
  debounce_timer = seconds;   ﻿  ﻿  // Start the timer fresh
  _BIC_SR_IRQ(LPM4_bits | LPM3_bits);  ﻿  // Dump out of LPM4 and run the CPU

}
  
//*************************************************************************************
//﻿  function:  TimerA_ISR
//﻿  returns:  Nothing.  Turn off motor if it's been on too long
//﻿  ﻿    tracks time and increments the seconds counter
//*************************************************************************************
interrupt (TIMERA0_VECTOR) TimerA_ISR(void) {  
 ticks++;﻿  ﻿  ﻿  ﻿  ﻿  // Smallest value of counter:  8.55mS    
  
  if (ticks >= 117) {﻿  ﻿  ﻿  ﻿  // .5Hz       
    P1OUT ^= TICKER;﻿  ﻿  ﻿  ﻿  // Toggles pin for tweaking     
    seconds++;﻿  ﻿  ﻿  ﻿  ﻿  // Increment every second
    ticks = 0;﻿  ﻿  ﻿  ﻿  ﻿  // Reset ticks counter
    if (seconds >= (RUN_TIME-1) ) {﻿  ﻿  // How long to let motor run      
      motorOff();       
    }
  } 
  CCR0 += 100;﻿  ﻿  ﻿  ﻿  ﻿  // Reset the CCR0 Register   
}

//*************************************************************************************
//﻿  function:  motorOff
//﻿  returns:  Nothing.  actually turns off motor and resets timers
//﻿  ﻿    Go back to sleep deep sleep (mode 4)
//*************************************************************************************
void motorOff() {
  P1OUT &= 0x02;﻿  ﻿  ﻿  ﻿  // Turn off the motor 
  seconds = 0;﻿  ﻿  ﻿  ﻿  ﻿  // reset the seconds counter 
  debounce_timer = seconds;﻿  ﻿  ﻿  // Reset the timer
  ticks = 0;﻿  ﻿  ﻿  ﻿  ﻿  // Reset tick
  _BIS_SR(LPM4_bits + GIE); ﻿  ﻿  ﻿  // Enter LPM4    
}

//*************************************************************************************
//﻿  function:  motorOn
//﻿  returns:  Nothing.  Turns on motor and starts reaper timer
//﻿  ﻿    Goes to light sleep (mode 3)
//*************************************************************************************
void motorOn() {
  P1OUT |= 0x03;﻿  ﻿  ﻿  ﻿  // Turn it on
  seconds = 0;﻿  ﻿  ﻿  ﻿  ﻿  // Start the seconds counter fresh 
  debounce_timer = seconds;﻿  ﻿  ﻿  // Reset the timer
  _BIS_SR(LPM3_bits + GIE); ﻿  ﻿  ﻿  // Enter LPM4  
}

//*************************************************************************************
//﻿  function:﻿  main
//﻿  returns:﻿  Nothing.  Drops into an endless for loop.  Only runs when an interrupt
//﻿  ﻿  ﻿  drops it back into active mode
//*************************************************************************************
int main(void) {
  WDTCTL = WDTPW + WDTHOLD; ﻿  ﻿  // Stop watchdog timer  
  P1DIR |= MOTOR | SENSOR | TICKER;﻿  // All outputs 
  P1OUT |= 0x02﻿  ;﻿  ﻿  ﻿  // Motor Off, Sensor On, Ticker off
  P1IE |= BUTTON; ﻿  ﻿  ﻿  // P1.3 interrupt enabled
  P1IFG &= ~BUTTON; ﻿  ﻿  ﻿  // P1.3 IFG cleared  
  CCTL0 = CCIE; ﻿  ﻿  ﻿  // CCR0 interrupt enabled  
  BCSCTL3 |= LFXT1S_2;﻿  ﻿  ﻿  // LFXT1 = VLO
  IFG1 &= ~OFIFG;﻿  ﻿  ﻿  // Clear OSCFault flag  
  CCR0 = 100;﻿  ﻿  ﻿  ﻿  // Set default value for CCR0.  Timer_A counts down from this
  TACTL = TASSEL_1 + MC_2; ﻿  ﻿  // ACLK, continuous mode for Timer_A  
  P1OUT &= 0x02 ;﻿  ﻿  ﻿  // Turn off motor but leave SENSOR alone
  P1IES = 0x01;﻿  ﻿  ﻿  ﻿  
   _BIS_SR(LPM4_bits + GIE); ﻿  ﻿  ﻿  // Enter LPM4.  The ISR will wake us if needed
  
  for(;;) {﻿  ﻿  ﻿  ﻿  ﻿  //  Loop forever

    if(buttonFlag) {﻿  ﻿  ﻿  ﻿  // We're awake and looking for an interrupt
     
      if ((seconds - debounce_timer) > 1) {﻿  // Debounce

﻿  if (!(P1OUT&0x01) ) {﻿  ﻿  ﻿  // If the motor is currently not running
﻿    motorOn(); 
﻿  } 
  
﻿  else  {﻿  ﻿  ﻿  ﻿  ﻿  // If the motor is currently running   
﻿    motorOff();
﻿  }
﻿  buttonFlag = 0;﻿  ﻿  ﻿  ﻿  // Reset the interrupt flag
      }
    }   
  }
}


