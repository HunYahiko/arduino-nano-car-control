#include <typedef.h>


state getStateOf(element e)
{
	bool doorOpenSensor        = (bool) !digitalRead(iDoorOpen);
	bool doorClosedSensor      = (bool) !digitalRead(iDoorClosed);
	
	bool platformInsideSensor  = (bool) !digitalRead(iPlatformIn);
	bool platformOutsideSensor = (bool) !digitalRead(iPlatformOut);

	bool rampOpenSensor        = (bool) !digitalRead(iRampOpen);
	bool rampClosedSensor      = (bool) !digitalRead(iRampClosed);

	state resultState;

	switch(e)
	{
		case DOOR:
			if(doorOpenSensor == doorClosedSensor)
				resultState = DOOR_ERROR;

			if(doorOpenSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_OPEN;
			else
				if(doorClosedSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_CLOSED;
			break;


		case PLATFORM:
			if(platformInsideSensor == platformOutsideSensor)
				resultState = PLATFORM_ERROR;

			if(platformInsideSensor == true && platformInsideSensor != platformOutsideSensor)
				resultState = PLATFORM_OPEN;
			else
				if(platformOutsideSensor == true && platformInsideSensor != platformOutsideSensor)
				resultState = PLATFORM_CLOSED;
			break;


		case RAMP:
			if(doorOpenSensor == doorClosedSensor)
				resultState = DOOR_ERROR;

			if(doorOpenSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_OPEN;
			else
				if(doorClosedSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_CLOSED;
			break;
	}
	return resultState;
}



state getActualState()
{
	if(getStateOf(DOOR) == DOOR_OPEN)
	{
		if(getStateOf(PLATFORM) == PLATFORM_OPEN)
		{
			if(getStateOf(RAMP) == RAMP_OPEN)
				return RAMP_OPEN;
			else
				return RAMP_CLOSED;
		}
		else
			return PLATFORM_CLOSED;
	}
	else
		return DOOR_CLOSED;
}



void vButtonClick()
{
	switch(action)
	{
		case CLOSE:
			prevAction = CLOSE;
			action = PAUSE;
			break;

		case OPEN:
			prevAction = OPEN;
			action = PAUSE;
			break;

		case PAUSE:
			switch(prevAction)
			{
				case OPEN:
					action = CLOSE;
					break;

				case CLOSE:
					action = OPEN;
					break;
			
			}
			prevAction = PAUSE;
			break;
	}
}



void vGetNextState()
{
	switch(action)
	{
		case OPEN:
			switch(actualState)
			{
				case DOOR_CLOSED:
					nextState = DOOR_OPEN;
					break;

				case DOOR_OPEN:
					nextState = PLATFORM_OPEN;
					break;

				case PLATFORM_CLOSED:
					nextState = PLATFORM_OPEN;
					break;

				case PLATFORM_OPEN:
					nextState = RAMP_OPEN;
					break;

				case RAMP_CLOSED:
					nextState = RAMP_OPEN;
					break;

				case RAMP_OPEN:
					action = PAUSE;
					break;
			}
			break;

		case PAUSE:
			if(prevAction != PAUSE)
				// STOP MOTORS
				activated = false;
			break;

		case CLOSE:
			switch(actualState)
			{
				case DOOR_CLOSED:
					if(actualState == nextState)
						action = PAUSE;
					else
						nextState = DOOR_OPEN;
					break;

				case DOOR_OPEN:
					if(actualState == nextState)
						nextState = DOOR_CLOSED;
					else
						nextState = PLATFORM_CLOSED;
					break;

				case PLATFORM_CLOSED:
					nextState = DOOR_CLOSED;
					break;

				case PLATFORM_OPEN:
					if(actualState == nextState)
						nextState = PLATFORM_CLOSED;
					else
						nextState = RAMP_CLOSED;
					break;

				case RAMP_CLOSED:
					nextState = PLATFORM_CLOSED;
					break;

				case RAMP_OPEN:
					nextState = RAMP_CLOSED;
					break;
			}
			break;
	}
}



u32 getDistance()
{
	digitalWrite(iProximityTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(iProximityTrig, HIGH);
	delayMicroseconds(10);
	digitalWrite(iProximityTrig, LOW);
	long duration = pulseIn(iProximityEcho, HIGH);

	return (duration * 0.034) / 2;
}

bool debounce(int pin, int debounceLimit) {
}
