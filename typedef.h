#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

typedef struct Motor
{
	u8 iPinRight;
	u8 iPinLeft;
} Motor;

typedef enum element
{
	DOOR,
	PLATFORM,
	RAMP

} element;

typedef enum state
{
	DOOR_CLOSED,

	DOOR_OPEN,
	PLATFORM_CLOSED,

	PLATFORM_OPEN,
	RAMP_CLOSED,

	RAMP_OPEN,

} state;

typedef enum event
{
	OPEN,
	PAUSE,
	CLOSE
} event;

typedef unsigned char  u8;

/* FUNCTION PROTOTYPES */




#endif



// c -> constant
// #define NC_NUME 132
// p -> pointer
// v -> void
