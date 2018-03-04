#include "Arduino.h"

class MyDisplay
{
	byte vram[4]; 
	const byte blank_idx = 10;
	const byte SEGMENT_MAP[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, /*blank*/ 0x00}; /* Segment byte maps for numbers 0 to 9 */
	const byte SEGMENT_SELECT[4] = {0x0E, 0x0D, 0x0B, 0x07}; /* Byte maps to select digit 1 to 4 */
	
	public:
		MyDisplay();
		void print(u16 val);
		void refresh();
		
	private:
		void WriteNumberToSegment(byte Segment, byte Value);
		void WriteValueToSegment(byte Segment, byte Value);
		void replace_zeros_on_left(u8 idx);
		
} ;
