

#include <my.h>

static uint8 myLocale;

// Returns locale of this device
uint8 getMyLocale()
{
    return myLocale;
}

// Sets locale of this device
void setMyLocale(uint8 locale)
{
    myLocale = locale;
}