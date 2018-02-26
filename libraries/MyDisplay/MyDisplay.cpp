#include <MyDisplay.hpp>

#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

MyDisplay::MyDisplay()
{
	//display
	pinMode(LATCH_DIO, OUTPUT);
	pinMode(CLK_DIO, OUTPUT);
	pinMode(DATA_DIO, OUTPUT);
	
	vram[0] = blank_idx ;
	vram[1] = blank_idx ;
	vram[2] = blank_idx ;
	vram[3] = blank_idx ;
}

void MyDisplay::write(u8 val)
{
	
	vram[0] = val %10;
	vram[1] = (val/10) %10;
	vram[2] = (val/100) %10;
	vram[3] = (val/1000) %10;
}

void MyDisplay::refresh()
{
	for(int i=0; i<4; i++)
	{
		//WriteNumberToSegment(i,vram[i]);
	}
	WriteNumberToSegment(0,vram[3]);
	WriteNumberToSegment(1,vram[2]);
	WriteNumberToSegment(2,vram[1]);
	WriteNumberToSegment(3,vram[0]);
}


/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void MyDisplay::WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO, HIGH);
}

/* Write a value to one of the 4 digits of the display */
void MyDisplay::WriteValueToSegment(byte Segment, byte Value)
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