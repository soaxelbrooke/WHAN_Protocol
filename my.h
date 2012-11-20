
// Include systems specific type def
#include "cytypes.h"
// Include for system specific prototypes (like memcpy)
#include "device.h"

#include "message.h"

// Uncomment this if the device is a coordinator
// #define COORDINATOR 1

// Uncomment this if the device is an actor (acts on notifications)
#define ACTOR 1
// Uncomment this if the device is a sensor (sends notifications)
// #define SENSOR 1

// Define address book related constants
#define MY_ADDRESS_BOOK_LENGTH 64
#define MY_ADDRESS_BOOK_INTEREST_LIMIT MY_NUM_CAPABILITIES
#define MY_ADDRESS_BOOK_CAPABILITY_LIMIT MY_NUM_INTERESTS


#define MAX_SINGLE_MESSAGE_LENGTH 75 // TODO: What's the actual number for this?


#define MY_DEVICE_TYPE 0

// Defines the total number of types of data this node can measure
#define MY_NUM_CAPABILITIES 0
// Defines the total number of types of data this node is interested in
#define MY_NUM_INTERESTS 2

// Returns locale of this device
uint8 getMyLocale();

// Sets locale of this device
void setMyLocale(uint8 locale);

// Returns a pointer to this node's interests
uint8 *getMyInterestsPtr(void);

// Returns a pointer to this node's capabilities
uint8 *getMyCapabilitiesPtr(void);

// Returns a pointer to this node's interests locales
uint8 *getMyInterestsLocalesPtr(void);