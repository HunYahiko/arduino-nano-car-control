#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

struct Motor
{
	u8 iPinRight;
	u8 iPinLeft;
};

typedef enum element
{
	DOOR,
	PLATFORM,
	RAMP

} element;

typedef enum state
{
	DOOR_OPEN,

	DOOR_CLOSED,
	PLATFORM_OPEN,

	PLAFORM_CLOSED,
	RAMP_OPEN,

	RAMP_CLOSED,
} state;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/* FUNCTION PROTOTYPES */




#endif



// c -> constant
// #define NC_NUME 132
// p -> pointer
// v -> void
