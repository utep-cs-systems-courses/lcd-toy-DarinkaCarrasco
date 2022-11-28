#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "sound.h"
// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15


int period = 500;
char sontT= 0;
/*
char blue = 31, green = 0, red = 31;
unsigned char step = 0;
*/
static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}
/*1
void
screen_update_ball()
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPixel[axis] != controlPixel[axis]) /* position changed? */
      /*    goto redraw;2
  return;			/* nothing to do */
      /* redraw:3
  draw_ball(drawPos[0], drawPos[1], COLOR_BLUE); /* erase */
/*for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* draw */


short redrawScreen = 1;

//u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen =1;
  }
}
    /*
    {				// move ball 
      short oldCol = controlPos[0];
      short newCol = oldCol + colVelocity;
      if (newCol <= colLimits[0] || newCol >= colLimits[1])
	colVelocity = -colVelocity;
      else
	controlPos[0] = newCol;
    }

    {				/* update hourglass 
      if (switches & SW3) green = (green + 1) % 64;
      if (switches & SW2) blue = (blue + 2) % 32;
      if (switches & SW1) red = (red - 3) % 32;
      if (step <= 30)
	step ++;
      else
	step = 0;
      secCount = 0;
    }
    if (switches & SW4) return;
    redrawScreen = 1;
  }
}
    */
void update_shape();

int main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
  return 0;
}

unsigned int SHAPE_COLOR = COLOR_DARK_VIOLE;

void update_shape(){
  
  static unsigned char row = screenHeight / 2 , col = screenWidth / 2;
  static int colStep = 5;
  static int rowStep = 5;
  static unsigned char step = 0;
  if(step <= 10)
    {
      int startCol = col - step*2;
      int endCol = col*2 + step*2;
      int width = 4 + endCol - startCol;
      
      //shape drawing, pressing the bottns will chage
      
      fillRectangle(endCol, row+1 -step, width, 1, SHAPE_COLOR);
      fillRectangle(endCol, row+1 +step, width, 1, SHAPE_COLOR);
      drawPixel(endCol+3, row, SHAPE_COLOR);
      drawPixel(endCol+3, row-1, SHAPE_COLOR);
      drawPixel(endCol+3, row+1, SHAPE_COLOR);
      drawPixel(endCol+3, row-2, SHAPE_COLOR);
      drawPixel(endCol+3, row+2, SHAPE_COLOR);
      drawPixel(endCol+3, row-3, SHAPE_COLOR);
      drawPixel(endCol+3, row+3, SHAPE_COLOR);

      fillRectangle(endCol+30, row*step, width, 1, COLOR_BLACK);

      if(switches & SW1) {
	SHAPE_COLOR = COLOR_RED;
	buzzer_set_period(0);
      }
      if(switches & SW2){
	SHAPE_COLOR = COLOR_BLUE;
	buzzer_set_period(0);
      }
      if(switches & SW3){
	SHAPE_COLOR = COLOR_GREEN;
	buzzer_set_period(0);
      }
      if(switches & SW4){
	playSong();
      }
      step ++;

    }
  else{
    col -= colStep;
    
    clearScreen(COLOR_BLUE);
    step =0;
  }
}
  /*
      screen_update_hourglass(){
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char lastStep = 0;
  
  if (step == 0 || (lastStep > step)) {
    clearScreen(COLOR_BLUE);
    lastStep = 0;
  } else {
    for (; lastStep <= step; lastStep++) {
      int startCol = col - lastStep;
      int endCol = col + lastStep;
      int width = 1 + endCol - startCol;
      
      // a color in this BGR encoding is BBBB BGGG GGGR RRRR
      unsigned int color = (blue << 11) | (green << 5) | red;
      
      fillRectangle(startCol, row+lastStep, width, 1, color);
      fillRectangle(startCol, row-lastStep, width, 1, color);
    }
  }
      }
}  


    
void
update_shape()
{
  screen_update_ball();
  screen_update_hourglass();
}
   

  */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
