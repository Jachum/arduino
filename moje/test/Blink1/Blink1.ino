#include <uTimerLib.h>

#include <TimerOne.h>
#include <MyDisplay.hpp>
//#include <Wire.h>

const u8 POT = 0;
const u8 led_1 = 13;
const u8 led_2 = 12;
const u8 led_3 = 11;
const u8 led_4 = 10;
const u32 sig_length = 50; //us, treat it as volume? 50000 seems ok.

const u32 SECOND_IN_US = 60 * 1000000;
u32 tempo = 120; //bpm
volatile bool is_tick = false;

#define BUZZER_DIO 3

volatile bool light1  = LOW;
volatile bool light2  = LOW;
byte value   = 0;

MyDisplay disp;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  //serial
  //Serial.begin(9600);

  //buzzer
  pinMode(BUZZER_DIO, OUTPUT);
  digitalWrite(BUZZER_DIO, HIGH);

  //timer
  Timer1.initialize(bpm2us(tempo));
  Timer1.attachInterrupt(timer_cb);
  //Timer1.stop();
  Timer1.start();

}

// the loop function runs over and over again forever
void loop() {

  //int pot_value = analogRead(POT);
  // Serial.print("Potentiometer reading: ");
  // Serial.println(pot_value);

  disp.print(tempo);
  disp.refresh();
  
}

void timer_cb()
{
  light1 = !light1;
  digitalWrite(led_1, light1);

  digitalWrite(BUZZER_DIO, LOW); //coment this to get silent mode 
  TimerLib.setInterval_us(timer2_cb, sig_length);
}

void timer2_cb()
{
  TimerLib.clearTimer(); 
  digitalWrite(BUZZER_DIO, HIGH);
  
  light2 = !light2;
  digitalWrite(led_2, light2);
}


u32 bpm2us(u32 bpm)
{
  return SECOND_IN_US / bpm;
}

u32 bpm2ms(u32 bpm)
{
  return bpm2us(bpm) / 1000;
}

u32 bpm2s(u32 bpm)
{
  return bpm2ms(bpm) / 1000;
}

