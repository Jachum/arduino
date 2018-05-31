//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int Value = 0;        // value read from analog input


void setup()
{
 lcd.begin(8, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("test2"); // print a simple message

 Serial.begin(57600);
}
 
void loop()
{
 lcd.setCursor(0,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up

 //Value = millis()/1000;
 //lcd.print(Value);
 //lcd.print("   ");
 //Serial.print("The analog input is: " );
 //Serial.print(Value);
 //Serial.println(" mv" );
}


