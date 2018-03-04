#include "Arduino.h"

class MyDigitalButton
{
	public:
		MyDigitalButton(u8 pin_nr, bool hi_or_low):pin(pin_nr), logic(hi_or_low), press(0), release(0){};
		bool is_pressed();
		bool operator==(const bool& rhs)
		    {
				if (is_pressed())
				{
					return rhs == logic;
				}
				else
				{
					return rhs != logic;
				}
		    }
		bool operator!=(const bool& rhs){ return !(*this == rhs); }

	private:
		u8 pin;
		u8 logic;
		u8 press;
		u8 release;
		const u8 bounce_time = 64; //should depend on MHz, really. Done for 16MHz cpu clock.

} ;
