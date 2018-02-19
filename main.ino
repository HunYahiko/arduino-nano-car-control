#include "typedef.h"

state actualState;
state nextState;
event action;
event prevAction;

bool activated = false; // indicates if any motor is working 

u8 iDoorControl;
u8 iButtonPin;

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


void setup()
{
	pinMode(iDoorOpen, INPUT);
	pinMode(iDoorClosed, INPUT);

	pinMode(iPlatformOpen, INPUT);
	pinMode(iPlatformClosed, INPUT);

	pinMode(iRampOpen, INPUT);
	pinMode(iRampClosed, INPUT);
	

	actualState = getActualState();

	attachInterrupt(iDoorOpen, <++>,LOW);
	attachInterrupt(iDoorClosed, <++>,LOW);

	attachInterrupt(iPlatformOpen, <++>,LOW);
	attachInterrupt(iPlatformClosed, <++>,LOW);

	attachInterrupt(iRampOpen, <++>,LOW);
	attachInterrupt(iRampClosed, <++>,LOW);

	attachInterrupt(iButtonPin, vButtonClick,LOW);

	action = PAUSE;
	prevAction = PAUSE;
}



void vButtonClick()
{
	switch(action)
	{
		case CLOSE:
			prevAction = CLOSE;
			action = OPEN;
			break;

		case OPEN:
			prevAction = OPEN;
			action = PAUSE;
			break;

		case PAUSE:
			if(actualState == DOOR_OPEN || actualState == PLATFORM_OPEN || actualState == RAMP_OPEN)
			{
				action = CLOSE;
				prevAction = PAUSE;
				break;
			}
			else
			{
				if(prevAction != CLOSE && (actualState == DOOR_CLOSED || actualState == PLATFORM_CLOSED || actualState == RAMP_CLOSED))
				{
					action = OPEN;
					prevAction = PAUSE;
					break;
				}
				else
					if(prevAction == CLOSE && (actualState == DOOR_CLOSED || actualState == PLATFORM_CLOSED || actualState ==
					RAMP_CLOSED))
					{
						action = CLOSE;
						prevAction = PAUSE;
						break;
					}
			}
	}
}


void vGetNextState
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
					prevAction = OPEN;
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
					nextState = DOOR_CLOSED;
					break;

				case DOOR_OPEN:
					if(actualState == nextState)
						nextState = DOOR_CLOSED;
					else
						nextState = PLATFORM_CLOSED;
					break;

				case PLATFORM_CLOSED:
					if(prevAction == PAUSE)
						nextState = PLATFORM_OPEN;
					else
						nextState = DOOR_CLOSED;
					break;

				case PLATFORM_OPEN:
					if(prevAction == PAUSE)
						nextState = RAMP_CLOSED;
						nextState = RAMP_OPEN;
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



void loop()
{
	if(digitalRead(iButtonPin))
	{
		vButtonClick();
	}
	

	if(action != PAUSE)
	{
		if(!activated)
			vGetNextState();

		switch(nextState)
		{
			case DOOR_CLOSED:
				if(!activated)
				{
					digitalWrite(motor[0].iPinRight, LOW);
					digitalWrite(motor[0].iPinLeft, HIGH);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iDoorClosed) == LOW)
					{
						digitalWrite(motor[0].iPinLeft, LOW);
						actualState = nextState;
						activated = false;
						action = PAUSE;
						prevAction = PAUSE;
					}
				}
				break;

			case DOOR_OPEN:
				if(!activated)
				{
					digitalWrite(motor[0].iPinRight, HIGH);
					digitalWrite(motor[0].iPinLeft, LOW);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iDoorOpen) == LOW)
					{
						digitalWrite(motor[0].iPinRight, LOW);
						actualState = nextState;
						activated = false;
					}
				}
				break;

			case PLATFORM_CLOSED:
				if(!activated)
				{
					digitalWrite(motor[1].iPinRight, HIGH);
					digitalWrite(motor[1].iPinLeft, LOW);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iPlatformClosed) == LOW)
					{
						digitalWrite(motor[1].iPinLeft, LOW);
						actualState = nextState;
						activated = false;
					}
				}
				break;

			case PLATFORM_OPEN:
				if(!activated)
				{
					digitalWrite(motor[1].iPinRight, HIGH);
					digitalWrite(motor[1].iPinLeft, LOW);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iPlatformOpen) == LOW)
					{
						digitalWrite(motor[1].iPinRight, LOW);
						actualState = nextState;
						activated = false;
					}
				}
				break;

			case RAMP_CLOSED:
				if(!activated)
				{
					digitalWrite(motor[2].iPinRight, HIGH);
					digitalWrite(motor[2].iPinLeft, LOW);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iRampOpen) == LOW)
					{
						digitalWrite(motor[2].iPinRight, LOW);
						actualState = nextState;
						activated = false;
					}
				}
				break;

			case RAMP_OPEN:
				if(!activated)
				{
					digitalWrite(motor[2].iPinRight, HIGH);
					digitalWrite(motor[2].iPinLeft, LOW);
					activated = true;
					break;
				}
				else
				{
					if(digitalRead(iRampClosed) == LOW)
					{
						digitalWrite(motor[2].iPinLeft, LOW);
						actualState = nextState;
						activated = false;
						action = PAUSE;
						prevAction = PAUSE;
						action = PAUSE;
						prevAction = PAUSE;
					}
				}
				break;
		}
	}


	
}
