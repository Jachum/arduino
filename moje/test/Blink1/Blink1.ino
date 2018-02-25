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

#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP[] = {~0xC0, ~0xF9, ~0xA4, ~0xB0, ~0x99, ~0x92, ~0x82, ~0xF8, ~0x80, ~0x90};
/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {~0xF1, ~0xF2, ~0xF4, ~0xF8};
/* Set DIO pins to outputs */

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

  //pot
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);

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

  int pot_value = analogRead(POT);
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

}


/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO, HIGH);
}

/* Write a value to one of the 4 digits of the display */
void WriteValueToSegment(byte Segment, byte Value)
{
  bitClear(PORTD, 4);

  for (uint8_t i = 0; i < 8; i++)  {
    bitWrite(PORTB, 0, Value & (1 << (7 - i)));
    bitSet(PORTD, 7);
    bitClear(PORTD, 7);
  }

  for (uint8_t i = 0; i < 8; i++)  {
    bitWrite(PORTB, 0, SEGMENT_SELECT[Segment] & (1 << (7 - i)));
    bitSet(PORTD, 7);
    bitClear(PORTD, 7);
  }

  bitSet(PORTD, 4);
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
  //WriteNumberToSegment(3 , (value++) % 10 );
  WriteValueToSegment(3, (value++) /*% 10*/);
  disp.refresh();
}

u32 bpm2us(u32 bpm)
{
  return SECOND_IN_US / bpm;
}

