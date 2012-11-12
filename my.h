
// Uncomment this if the device is a coordinator
// #DEFINE COORDINATOR 1

// Max number of nodes that this node keeps track of
#define MY_ADDRESS_BOOK_LENGTH 64

#define MY_DEVICE_TYPE 0

// Defines the total number of types of data this node can measure
#define MY_NUM_CAPABILITIES 3
// Defines the total number of types of data this node is interested in
#define MY_NUM_INTERESTS 0

// Returns locale of this device
uint8 getMyLocale();

// Sets locale of this device
void setMyLocale(uint8 locale);