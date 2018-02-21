#include "typedef.h"

Motor motor[3];
motor[0].iPinRight = ;
motor[0].iPinLeft  = ;

motor[1].iPinRight = ;
motor[1].iPinLeft  = ;

motor[2].iPinRight = ;
motor[2].iPinLeft  = ;


u8 iDoorOpen   = ;
u8 iDoorClosed = ;

u8 iPlatformIn  = ;
u8 iPlatformOut = ;

u8 iProximityTrig = ;
u8 iProximityEcho = ;

u8 iRampOpen   = ;
u8 iRampClosed = ;


state actualState;
state nextState;

event action;
event prevAction;

bool activated = false; // indicates if any motor is working 

u8 iButtonPin =;




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
	// To Do: write a function to set prevAction
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
					}
				}
				break;
		}
	}


	
}
