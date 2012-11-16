
// Include systems specific type def
#include "cytypes.h"
// Include for system specific prototypes (like memcpy)
#include "device.h"

// Uncomment this if the device is a coordinator
// #DEFINE COORDINATOR 1

// Define address book related constants
#define MY_ADDRESS_BOOK_LENGTH 64
#define MY_ADDRESS_BOOK_INTEREST_LIMIT MY_NUM_CAPABILITIES
#define MY_ADDRESS_BOOK_CAPABILITY_LIMIT MY_NUM_INTERESTS


#define MY_DEVICE_TYPE 0

// Defines the total number of types of data this node can measure
#define MY_NUM_CAPABILITIES 3
// Defines the total number of types of data this node is interested in
#define MY_NUM_INTERESTS 1

// Returns locale of this device
uint8 getMyLocale();

// Sets locale of this device
void setMyLocale(uint8 locale);
