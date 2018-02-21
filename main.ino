#include "typedef.h"



Motor motor[3];
motor[0].iPinRight = 3;
motor[0].iPinLeft  = 5;

motor[1].iPinRight = 6;
motor[1].iPinLeft  = 9;

motor[2].iPinRight = 10;
motor[2].iPinLeft  = 11;



u8 iDoorOpen   = 14;
u8 iDoorClosed = 15;

u8 iPlatformIn  = 16;
u8 iPlatformOut = 17;

u8 iRampOpen   = 18;
u8 iRampClosed = 19;

u8 iButtonPin = 4;

u8 iProximityTrig = 7;
u8 iProximityEcho = 8;

NewPing sonar(iProximityTrig, iProximityEcho, 20);

state actualState;
state nextState;

event action;
event prevAction;

bool activated = false; // indicates if any motor is working 



void setup()
{
	pinMode(iDoorOpen, INPUT);
	pinMode(iDoorClosed, INPUT);

	pinMode(iPlatformOpen, INPUT);
	pinMode(iPlatformClosed, INPUT);

	pinMode(iRampOpen, INPUT);
	pinMode(iRampClosed, INPUT);

	pinMode(iButtonPin, INPUT);

	pinMode(motor[0].iPinRight, OUTPUT);
	pinMode(motor[0].iPinLeft, OUTPUT);

	pinMode(motor[1].iPinRight, OUTPUT);
	pinMode(motor[1].iPinLeft, OUTPUT);

	pinMode(motor[2].iPinRight, OUTPUT);
	pinMode(motor[2].iPinLeft, OUTPUT);
	
	actualState = getActualState();

	action = PAUSE;
	prevAction = getPrevAction();
}



void loop()
{
	if(debounce(iButtonPin, 100, 1))
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
					if(debounce(iDoorClosed, 50, 0) == true)
					{
						digitalWrite(motor[0].iPinLeft, LOW);
						actualState = nextState;
						activated = false;
						action = PAUSE;
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
					if(debounce(iDoorOpen, 50, 0) == true)
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
					if(debounce(iPlatformClosed, 50, 0) == true)
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
					if(debounce(iPlatformOpen, 50, 0) == true)
					{
						digitalWrite(motor[1].iPinRight, LOW);
						actualState = nextState;
						activated = false;
					}
					else
					{
						if(sonar.ping_cm() < 20 && sonar.ping_cm() != 0)
						{
							digitalWrite(motor[1].iPinRight, LOW);
							action = PAUSE;
							prevAction = CLOSE;
							activated = false;
						}
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
					if(debounce(iRampOpen, 50, 0) == true)
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
					if(debounce(iRampClosed, 50, 0) == true)
					{
						digitalWrite(motor[2].iPinLeft, LOW);
						actualState = nextState;
						activated = false;
						action = PAUSE;
					}
				}
				break;
		}
	}

LOW
	
}
