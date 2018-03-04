#include <uTimerLib.h>

#include <TimerOne.h>
#include <MyDisplay.hpp>
#include <MyDigitalButton.hpp>
//#include <Wire.h>

#define BUZZER_DIO 3

const u8 POT = 0;
const u8 led_1 = 13;
const u8 led_2 = 12;
const u8 led_3 = 11;
const u8 led_4 = 10;
const u32 sig_length = 30000; //us, treat it as volume? 50000 seems ok.

const u32 SECOND_IN_US = 60 * 1000000;
u16 tempo = 120; //bpm
volatile bool light1  = LOW;
volatile bool light2  = LOW;
MyDigitalButton btn1(A1, LOW); //down
MyDigitalButton btn2(A2, LOW); //up
MyDigitalButton btn3(A3, LOW); //reset
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

  if (btn1.is_pressed())
  {
    tempo--;
  }

  if (btn2 == LOW)
  {
      tempo++;
  }

  if (btn3 == LOW)
  {
    tempo = 120;
  }

  tempo = limit(tempo);
  Timer1.setPeriod(bpm2us(tempo));

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

u16 limit(u16 val)
{
  if (val < 1)
  {
    val = 1;
  }
  else if (val > 300)
  {
    val = 300;
  }

  return val;
}

