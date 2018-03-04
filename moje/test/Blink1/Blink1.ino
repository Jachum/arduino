#include <uTimerLib.h>

#include <TimerOne.h>
#include <MyDisplay.hpp>
//#include <Wire.h>

#define BUZZER_DIO 3

const u8 POT = 0;
const u8 led_1 = 13;
const u8 led_2 = 12;
const u8 led_3 = 11;
const u8 led_4 = 10;
const u32 sig_length = 50; //us, treat it as volume? 50000 seems ok.

const u32 SECOND_IN_US = 60 * 1000000;
u16 tempo = 120; //bpm
volatile bool light1  = LOW;
volatile bool light2  = LOW;
u8 btn1_press   = 0;
u8 btn1_release = 0;
u8 btn2_press   = 0;
u8 btn2_release = 0;
u8 btn3_press   = 0;
u8 btn3_release = 0;
const u8 bounce_time  = 64;
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

  bool button1 = digitalRead(A1);
  if (button1 == LOW)
  {
    btn1_press++;
    btn1_release = 0;
    if (btn1_press >= bounce_time)
    {
      tempo--;
      btn1_press = 0;
    }
  }
  else
  {
    btn1_release++;
    if (btn1_release >= bounce_time)
    {
      btn1_press = 0;
    }

  }

  bool button2 = digitalRead(A2);
  if (button2 == LOW)
  {
    btn2_press++;
    if (btn2_press >= bounce_time)
    {
      tempo++;
      btn2_press = 0;
    }
  }

  bool button3 = digitalRead(A3);
  if (button3 == LOW)
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

