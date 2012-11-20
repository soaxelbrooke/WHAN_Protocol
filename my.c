

#include "my.h"

static uint8 myLocale;
uint8 myInterests[] = {UNIT_TYPE_LUMENS, UNIT_TYPE_OCCUPANTS};
uint8 myInterestsLocales[] = {0, 0};
uint8 myCapabilities[] = {0};

// Returns locale of this device
uint8 getMyLocale()
{
    return myLocale;
}

// Sets locale of this device
void setMyLocale(uint8 locale)
{
    myLocale = locale;
    // TODO:
	// UPDATE INTERESTS LOCALES
	// Also, do what needs to be done with the address book...
	// Resend interest notification?
}

// Returns a pointer to this node's interests
uint8 *getMyInterestsPtr(void)
{
	return (uint8*)(&myInterests);
}

// Returns a pointer to this node's capabilities
uint8 *getMyCapabilitiesPtr(void)
{
	return (uint8*)(&myCapabilities);
}


// Returns a pointer to this node's interests locales
uint8 *getMyInterestsLocalesPtr(void)
{
	return (uint8*)(&myInterestsLocales);
}
