
#include "cytypes.h"
//#include "LightController.h" // TODO: make this mofo.

// Define possible light commands
// See https://docs.google.com/document/d/1uqoRSOfVvMMXYMv4Y9xCXIrN8ot1bmZtFdnBIg_3UVE/edit
// for details
#define LIGHT_COMMAND_ON 0x00
#define LIGHT_COMMAND_RESET 0x80
#define LIGHT_COMMAND_OFF 0x40
#define LIGHT_COMMAND_NEXT_MODE 0x20
#define LIGHT_COMMAND_SLEEP 0xA0
#define LIGHT_COMMAND_TIMER 0x60
#define LIGHT_COMMAND_COLOR_MIXING 0x10
#define LIGHT_COMMAND_BRIGHTNESS_DOWN 0x90
#define LIGHT_COMMAND_BRIGHTNESS_UP 0x50
#define LIGHT_COMMAND_COLOR_RED 0x30
#define LIGHT_COMMAND_COLOR_GREEN 0xB0
#define LIGHT_COMMAND_COLOR_BLUE 0x70
#define LIGHT_COMMAND_COLOR_ORANGE 0x08
#define LIGHT_COMMAND_COLOR_SEAFOAM 0x88
#define LIGHT_COMMAND_COLOR_AZURE 0x48
#define LIGHT_COMMAND_COLOR_PURPLE 0x28
#define LIGHT_COMMAND_COLOR_YELLOW 0xA8
#define LIGHT_COMMAND_COLOR_SKYBLUE 0x68
#define LIGHT_COMMAND_COLOR_PINK 0x18
#define LIGHT_COMMAND_COLOR_CREAM 0x98
#define LIGHT_COMMAND_COLOR_WHITE 0x58

#define NUM_BRIGHTNESS_STEPS 16 // TODO: determine how many different brightness steps there are.

#define DIMMING_PER_BRIGHTNESS_STEP (0x7fffffff / NUM_BRIGHTNESS_STEPS)

// Initialize light data
void initLight(void);

// Send a command to the light, as chosen from the #define list above
void sendLightCommand(uint8 lightCommand);

// Send repeat code the specified number of times.  This should be called 
// shortly after the sendLightCommand function.  
void repeatCommand(uint16 numRepeats);

// Increase brightness
void increaseBrightness(int32 plusDimming);

// Decrease brightness
void decreaseBrightness(int32 minusDimming);

// Set brightness
void setBrightness(int32 dimming);

// Turn light on
void turnOn(void);

// Turn light off
void turnOff(void);

// Get the current light brightness
int32 getBrightness(void);

// Convert brightness to dimming
int32 brightnessToDimming(int32 brightness);

// Convert dimming to brightness
int32 dimmingToBrightness(int32 dimming);
