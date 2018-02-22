#include <NewPing.h>



struct Motor_t
{
	int iPinRight;
	int iPinLeft;
} motor[3];

typedef enum element_t
{
	DOOR,
	PLATFORM,
	RAMP

} element;

typedef enum state_t
{
	DOOR_CLOSED,

	DOOR_OPEN,
	PLAFORM_CLOSED,

	PLATFORM_OPEN,
	RAMP_CLOSED,

	RAMP_OPEN

} state;

typedef enum event
{
	OPEN,
	PAUSE,
	CLOSE
} event;


motor[0].iPinRight = 3;
motor[0].iPinLeft  = 5;

motor[1].iPinRight = 6;
motor[1].iPinLeft  = 9;

motor[2].iPinRight = 10;
motor[2].iPinLeft  = 11;



int iDoorOpen   = 14;
int iDoorClosed = 15;

int iPlatformOpen  = 16;
int iPlatformClosed = 17;

int iRampOpen   = 18;
int iRampClosed = 19;

int iButtonPin = 4;

int iProximityTrig = 7;
int iProximityEcho = 8;

NewPing sonar(iProximityTrig, iProximityEcho, 20);

state actualState;
state nextState;

event action;
event prevAction;

bool activated = false; // indicates if any motor is working 












// Detects the state of the car element (check typedef.h)
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
				resultState = DOOR_CLOSED;

			if(doorOpenSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_OPEN;
			else
				if(doorClosedSensor == true && doorOpenSensor != doorClosedSensor)
				resultState = DOOR_CLOSED;
			break;


		case PLATFORM:
			if(platformInsideSensor == platformOutsideSensor)
				resultState = PLATFORM_CLOSED;

			if(platformInsideSensor == true && platformInsideSensor != platformOutsideSensor)
				resultState = PLATFORM_OPEN;
			else
				if(platformOutsideSensor == true && platformInsideSensor != platformOutsideSensor)
				resultState = PLATFORM_CLOSED;
			break;


		case RAMP:
			if(doorOpenSensor == doorClosedSensor)
				resultState = DOOR_CLOSED;

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

action getPrevAction()
{
	switch(actualState)
	{
		case DOOR_CLOSED:
			return CLOSE;

		case DOOR_OPEN:
			return OPEN;

		case PLATFORM_CLOSED:
			return CLOSE;

		case PLATFORM_OPEN:
			return OPEN;

		case RAMP_CLOSED:
			return CLOSE;

		case RAMP_OPEN:
			return OPEN;
	}
}


void stopMotors()
{
	digitalWrite(motor[0].iPinRight, LOW);
	digitalWrite(motor[0].iPinLeft, LOW);
	digitalWrite(motor[1].iPinRight, LOW);
	digitalWrite(motor[1].iPinLeft, LOW);
	digitalWrite(motor[2].iPinRight, LOW);
	digitalWrite(motor[2].iPinLeft, LOW);
	activated = false;
}


void vButtonClick()
{
	switch(action)
	{
		case CLOSE:
			prevAction = CLOSE;
			action = PAUSE;
			stopMotors();
			break;

		case OPEN:
			prevAction = OPEN;
			action = PAUSE;
			stopMotors();
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
			//prevAction = PAUSE;
			break;
	}
}



// Depending on the action that should be done, selects the nextState
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
					if(actualState == nextState)
						nextState = PLATFORM_OPEN;
					else
						nextState = DOOR_OPEN;
					break;

				case PLATFORM_OPEN:
					nextState = RAMP_OPEN;
					break;

				case RAMP_CLOSED:
					if(actualState == nextState)
						nextState = RAMP_OPEN;
					else
						nextState = PLATFORM_OPEN;
					break;

				case RAMP_OPEN:
					if(actualState == nextState)
						action = PAUSE;
					else
						nextState = RAMP_OPEN;
					break;
			}
			break;

		case CLOSE:
			switch(actualState)
			{
				case DOOR_CLOSED:
					if(actualState == nextState)
						action = PAUSE;
					else
						nextState = DOOR_CLOSED;
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



 bool debounce(int pin, int debounceLimit, int front ) {

     static int debounceLimitIncrement;
     int current = digitalRead(pin);
        if(front != current) {
            debounceLimitIncrement = 0;
        }
        if(front == current) {
            debounceLimitIncrement++;
        }
        if(debounceLimitIncrement >= debounceLimit) {
            return true;
        } else {
            debounce(pin,debounceLimit,front);
        }
 }













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
}
