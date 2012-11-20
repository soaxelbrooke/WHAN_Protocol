
#include "light_control.h"

int32 currentBrightness;

// Initialize light data
void initLight(void)
{
    sendLightCommand(LIGHT_COMMAND_RESET);
	currentBrightness = NUM_BRIGHTNESS_STEPS - 1;
	
}

// Send a command to the light, as chosen from the #define list above
void sendLightCommand(uint8 lightCommand)
{
	// TODO: create light control component, utilize it here!
}

// Send repeat code the specified number of times.  This should be called 
// shortly after the sendLightCommand function.  
void repeatCommand(uint16 numRepeats)
{
	// TODO: Make sure to plan for repeat codes in the component.
}

// Increase brightness
void increaseBrightness(int32 plusDimming)
{
	// Convert dimming to light brightness steps, send increase brightness 
	// command that many times.
	
	int32 plusBrightness = dimmingToBrightness(plusDimming);
	
	// Ensure brightness doesn't go over upper limit
	if ((currentBrightness + plusBrightness) > (NUM_BRIGHTNESS_STEPS - 1))
	{
		plusBrightness = (NUM_BRIGHTNESS_STEPS - 1) - currentBrightness;
		
	}
	
	// Implement brightness change
	if (plusBrightness > 1)
	{
		sendLightCommand(LIGHT_COMMAND_BRIGHTNESS_UP);
		repeatCommand(plusBrightness - 1);
	
	}
	else if (plusBrightness == 1)
	{
		sendLightCommand(LIGHT_COMMAND_BRIGHTNESS_UP);
	
	}
	
	// Update brightness status
	currentBrightness += plusBrightness;
	
}

// Decrease brightness
void decreaseBrightness(int32 minusDimming)
{
	// Convert dimming to light brightness steps, send decrease brightness 
	// command that many times.
	
	int32 minusBrightness = dimmingToBrightness(minusDimming);
	
	// Ensure brightness doesn't go below lower limit
	if ((currentBrightness - minusBrightness) < 0)
	{
		minusBrightness = currentBrightness;
		
	}
	
	// Implement brightness change
	if (minusBrightness > 1)
	{
		sendLightCommand(LIGHT_COMMAND_BRIGHTNESS_DOWN);
		repeatCommand(minusBrightness - 1);
	
	}
	else if (minusBrightness == 1)
	{
		sendLightCommand(LIGHT_COMMAND_BRIGHTNESS_DOWN);
	
	}
	
	// Update brightness status
	currentBrightness -= minusBrightness;
	
}

// Set brightness
void setBrightness(int32 dimming)
{
	// Solve for how many brightness changes are needed to get to target, then
	// do that many.
	
	int32 deltaBrightness = 0;
	int32 targetBrightness = dimmingToBrightness(dimming);
	
	// Determine number of brightness steps needed (and where target is 
	// achievable)
	if ((targetBrightness >= 0) & (targetBrightness < NUM_BRIGHTNESS_STEPS))
	{
		deltaBrightness = targetBrightness - currentBrightness ;
		
	}
	
	// Call increase/decrease functions depending on needed change
	if (deltaBrightness > 0)
	{
		increaseBrightness(deltaBrightness);
		
	}
	else if (deltaBrightness < 0)
	{
		decreaseBrightness(-deltaBrightness);
		
	}
	
}

// Turn light on
void turnOn(void)
{
	sendLightCommand(LIGHT_COMMAND_ON);
	
}

// Turn light off
void turnOff(void)
{
	sendLightCommand(LIGHT_COMMAND_OFF);

}

// Get the current light brightness
int32 getBrightness(void)
{
	return currentBrightness;
	
}

// Convert brightness to dimming
int32 brightnessToDimming(int32 brightness)
{
	return brightness * DIMMING_PER_BRIGHTNESS_STEP;
	
}

// Convert dimming to brightness
int32 dimmingToBrightness(int32 dimming)
{
	return dimming / DIMMING_PER_BRIGHTNESS_STEP;
	
}

/* [] END OF FILE */
