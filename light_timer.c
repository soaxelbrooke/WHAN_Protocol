
#include <light_timer.h>

// Starts the turnoff timer
void startTurnOffTimer(void)
{
    OnTimer_Stop();
	OnTimer_Start();
	
}

// Resets the turnoff timer to its initial state
void resetTurnOffTimer(void)
{
	OnTimer_WriteCounter(OnTimer_ReadPeriod());
	
}

// Sets the length of the turnoff timer in ms
void setTurnOffTimerPeriod(uint16 ms)
{
	OnTimer_WritePeriod(ms);
	
}

