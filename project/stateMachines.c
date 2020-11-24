#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "p2switches.h"

static char sb = 1; //Determines if we are on up or down state
static char toggle_state = 0; //Determines current toggle state for dimming
static char dim_state = 0; //Determines state of dim()
static char repeated = 0; //Used to control how many times funcitons are called in dim

void draw_diamonds()
{
  static char color_state = 0;
  switch (color_state) {
  case 0:
    drawDiamond(60,COLOR_CYAN);
    color_state++;
    break;
  case 1:
    drawDiamond(60,COLOR_BLUE);
    color_state++;
    break;
  case 2:
    drawDiamond(60,COLOR_PURPLE);
    color_state = 0;
    break;
  }   
}  
void turn_red_off() //Turns red off
{
  red_on = 0;
  led_update();
}

void toggle_red() //Red at 50% intensity		
{
  if (repeated < 2) {
    repeated++;
    switch (toggle_state) {
    case 0: //Red on
      red_on = 1;
      toggle_state = 1;
      break;
    case 1: //Red off
      red_on = 0;
      toggle_state = 0;
      break;
    }
    led_update();
  }
  else {
    repeated = 0;
    dim_state = 1;
  }   
}

void toggle_red75() //Red at 75% intensity
{
  if (repeated < 3) {
    repeated++;
    switch (toggle_state) {
    case 0: //Red on
      red_on = 1;
      toggle_state = 1;
      break;
    case 1: //Red on 
      red_on = 1;
      toggle_state = 2;
      break;
    case 2: //Red off
      red_on = 0;
      toggle_state = 0;
      break;
    }
    led_update();
  }
  else {
    repeated = 0;
    dim_state = 2;
  }   
}  

void toggle_red25() //Red at 25% intensity
{
  if (repeated < 3) {
    repeated++;
    switch (toggle_state) {
    case 0: //Red off
      red_on = 0;
      toggle_state = 1;
      break;
    case 1: //Red off
      red_on = 0;
      toggle_state = 2;
      break;
    case 2: //Red on
      red_on = 1;
      toggle_state = 0;
      break;
    }
    led_update();
  }
  else {
    repeated = 0;
    dim_state = 0;
  }  
}
  
void dim()
{
  switch (dim_state) {
  case 0: //Dim intensity 50
    toggle_red();
    break;
  case 1: //Dim intensity 75
    toggle_red75();
    break;
  case 2: //Dim intensity 25
    toggle_red25();
    break;
  }
}  

void turn_green_on() //Only green will be on
{
  green_on = 1;
  red_on = 0;
  led_update();
}

void turn_red_on() //Only red will be on
{
  red_on = 1;
  green_on = 0;
  led_update();
}

void up_state(){ //Up state in main_state_advance
  sb = 1;
  turn_green_on();
}

void down_state(){ //Down state in main_state_advance 
  sb = 0;
  turn_red_on();
}

void siren_state_advance(){ //Controls siren 
  static char state = 0;
  switch(state) {
  case 0:
  case 1: //Tone goes up in first 2 cases
    up_state();
    state++;
    break;
  case 2: //Tone goes down in last case
    down_state();
    state = 0;
  default:
    break;
  }
}

void buzzer_advance() //Plays siren
{
  static int x = 500;
  if (sb) { //Goes up if we are on up state
    x += 225;
  }
  else{ //Goes down if we are on down state
    x -= 450;
  }

  short cycles = 2000000 / x;
  buzzer_set_period(cycles);
}

void buzz_song_advance() //Plays Sonic song
{
  static char curr_note = 0;
  
  //Alternating Sonic sprites
  if ((curr_note % 2) == 0) 
    drawSonic(50,50,COLOR_WHITE, COLOR_BLUE); 
  else 
    drawSonic2(50,50,COLOR_WHITE, COLOR_BLUE);
     
  switch(curr_note) {
  case 0: //Plays note G
    buzzer_set_period(G);
    curr_note++;
    break;
  case 1: //Plays note E, hold 2 beats
    buzzer_set_period(E);
    curr_note++;
    break;
  case 3: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 4: //Plays note E
    buzzer_set_period(E);
    curr_note++;
    break;
  case 5: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 6: //Plays note F, hold 2 beats
    buzzer_set_period(F);
    curr_note++;
    break;
  case 8: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 9: //Plays note D, hold 2 beats
    buzzer_set_period(D);
    curr_note++;
    break;
  case 11: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 12: //Plays note E
    buzzer_set_period(E);
    curr_note++;
    break;
  case 13: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 14: //Plays note E
    buzzer_set_period(E);
    curr_note++;
    break;
  case 15: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 16: //Plays note E
    buzzer_set_period(E);
    curr_note++;
    break;
  case 17: //Break in notes
    buzzer_set_period(0);
    curr_note++;
  case 18: //Plays note C, hold 2 beats
    buzzer_set_period(Cn);
    curr_note++;
    break;
  case 20: //Break in notes
    buzzer_set_period(0);
    curr_note++;
    break;
  case 21: //Plays note A#, hold 2 beats
    buzzer_set_period(ASH);
    curr_note++;
    break;
  case 23: //Plays note D, hold 2 beats
    buzzer_set_period(D);
    curr_note++;
    break;
  case 25: //Song end, back to beggining
    buzzer_set_period(0);
    curr_note = 0;
    break;
  default:
    curr_note++;
  }   
}  
    
