#include <MyDigitalButton.hpp>
#include "Arduino.h"

bool MyDigitalButton::is_pressed()
{
	  bool result = false;

	  if (digitalRead(pin) == logic)
	  {
	    press++;
	    release = 0;
	    if (press >= bounce_time)
	    {
	      result = true;
	      press = 0;
	    }
	  }
	  else
	  {
	    release++;
	    if (release >= bounce_time)
	    {
	      press = 0;
	    }
	  }
}
