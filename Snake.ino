/*
SNAKE GAME 
Created by Michael Cameron Welnick
implimented on arduino UNO R3 using adafrui 8x8 bi-Color LED matrix
I2C clock connected to port A5
I2C data connected to port  A4
analog joystick x input connected to port A0
analog joystick y input connected to port A1
*/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);  // pass in the address

}

int8_t snake_move=3;   // initialize movement to right
int8_t snake_length=5; // initialize snake length
int8_t snake_x[32]={   // initial snake x positions
  0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7};
int8_t snake_y[32]={   // initial snake y positions
  0,1,2,3,4,5,6,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5};
int8_t apple_x=5;      //initial apple x position
int8_t apple_y=0;      // inital apple y position
int8_t collision = 0;
int break_free = 0;
int delta = 210; // time delay
int score = 0;
void loop() {
  
  randomSeed(millis());    // initialize random seed
  matrix.setRotation(3);   // set rotation
  Serial.println(score);
  
  // FOLLOWING
   for (int8_t t=31; t > 0; t--){
    snake_x[t]=snake_x[t-1];
    snake_y[t]=snake_y[t-1];
  }

  // MOVEMENT      
  switch (snake_move){
  case 1:  // up
    snake_y[0]--;
    break;

  case 2:  // down
    snake_y[0]++;
    break;

  case 3:  // right
    snake_x[0]++;
    break;

  case 4:  // left
    snake_x[0]--;
    break;
  }
  
  // Check for colision with self
  for (int8_t h=3; h<=snake_length; h++) // impossible to hit bits 0 through 2
  {
    if (snake_y[0] == snake_y[h] && snake_x[0] == snake_x[h]) 
    {
      collision = 1;
    }
  }

  //  COLLISION
  // first check for colision with self
  for (int8_t h=3; h<=snake_length; h++) // impossible to hit bits 0 through 2
  {
    if (snake_y[0] == snake_y[h] && snake_x[0] == snake_x[h]) 
    {
      collision = 1;
    }
  }
  
  if (snake_x[0] > 7 || snake_x[0] < 0 || snake_y[0] > 7 || snake_y[0] < 0 || collision == 1) // If it hits a wall or itself
  {
    for (int8_t z=0; z <= 1000; z++) // Blink 1000 times
    { 

      matrix.clear(); // clear the snake
      matrix.writeDisplay(); // write blank screen
      delay(200); // wait 
      matrix.clear();  
      for (int8_t i=0; i<snake_length; i++) // Light up the snake
      { 
        matrix.drawPixel(snake_x[i],snake_y[i],LED_RED);
      } 
      matrix.drawPixel(apple_x, apple_y, LED_GREEN); // Light up apple
      matrix.writeDisplay();  // Write the snake
      delay(200); // wait

    }
  } 

  // APPLE GRAB
  if (snake_x[0] == apple_x && snake_y[0] == apple_y)
  {
     snake_length++;
     break_free = 0;
     delta = delta - 5; 
     score = score + 100;
    while (break_free <= 0)  {
    break_free = 0;  
    apple_x= random(0,7);  
    apple_y= random(0,7);
    for (int8_t p=0; p<=snake_length; p++) {
  
     if (apple_y == snake_y[p] && apple_x == snake_x[p]) {
    
      break_free = break_free-- ; }
    }
     if (break_free == 0) {
      break_free = 1; }
    
  
  }
  }
  // CLEAR
  matrix.clear();

  // LIGHT UP SNAKE
  for (int8_t i=0; i<snake_length; i++){   // light up only length of snake
    matrix.drawPixel(snake_x[i],snake_y[i],LED_RED);
  }
  matrix.drawPixel(apple_x, apple_y, LED_GREEN);  // Light up apple

  // WRITE
  matrix.writeDisplay();  

      
  // DELAY AND READ CONTROLS
  for (int u = 0; u <  delta  ; u++)   // read voltage and update controls during delay
  {
    int sensorValue_x = analogRead(A0);  //read voltage
    int sensorValue_y = analogRead(A1);
    int ecks = sensorValue_x - 516 ;     // normalize
    int why = sensorValue_y - 516 ;  
    if (abs(ecks) >= abs(why))           // control update if statements
    {
      if (abs(ecks) > 250 && ecks >0 && snake_move != 3)
      { 
        snake_move = 4;  //left
      }
      else if (abs(ecks) > 250 && ecks < 0 && snake_move != 4)
      {
        snake_move = 3; // right
      }
    }
    else
    {
      if (abs(why) > 250 && why > 0 && snake_move != 2 )
      {
        snake_move = 1; //up
      }
      else if (abs(why) > 250 && why < 0 && snake_move != 1) 
      {
        snake_move = 2; // down
      }
    }
    delay(1); // delay
  }
}

