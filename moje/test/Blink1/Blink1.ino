#include <TimerOne.h>
#include <MyDisplay.hpp>
//#include <Wire.h>

const int POT = 0;
const int led_1 = 13;
const int led_2 = 12;
const int led_3 = 11;
const int led_4 = 10;

const u32 SECOND_IN_US = 60 * 1000000;

#define BUZZER_DIO 3



bool light = LOW;
byte value = 0;
u32 interval = 100000;
MyDisplay disp;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  //serial
  Serial.begin(9600);



  //buzzer
  pinMode(BUZZER_DIO, OUTPUT);
  digitalWrite(BUZZER_DIO, HIGH);
  //play_music();

  //timer
  Timer1.initialize(interval);
  Timer1.attachInterrupt(timer_cb);
  //Timer1.stop();
  Timer1.start();
}

// the loop function runs over and over again forever
void loop() {

  //int pot_value = analogRead(POT);
  /*
    const int delay_time = pot_value / 4;
    digitalWrite(led_1, LOW);
    delay(delay_time);
    digitalWrite(led_2, LOW);
    delay(delay_time);
    digitalWrite(led_3, LOW);
    delay(delay_time);
    digitalWrite(led_4, LOW);
    delay(delay_time);

    digitalWrite(led_1, HIGH);
    delay(delay_time);
    digitalWrite(led_2, HIGH);
    delay(delay_time);
    digitalWrite(led_3, HIGH);
    delay(delay_time);
    digitalWrite(led_4, HIGH);
    delay(delay_time);
  */
  // Serial.print("Potentiometer reading: ");
  // Serial.println(pot_value);

  /* Update the display with the current counter value */
  //  WriteNumberToSegment(0 , 0);
  //  WriteNumberToSegment(1 , 1);
  //  WriteNumberToSegment(2 , 2);
  //  WriteNumberToSegment(3 , 3);
  disp.write(145);
  disp.refresh();
}



void play_music()
{
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(BUZZER_DIO, LOW);
    delay(100);
    digitalWrite(BUZZER_DIO, HIGH);
    delay(100);
  }
}

void timer_cb()
{
  light = !light;
  digitalWrite(led_1, light);  
}

u32 bpm2us(u32 bpm)
{
  return SECOND_IN_US / bpm;
}

