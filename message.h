// TODO: put defines in here!


// Message type defines
#define MESSAGE_TYPE_USER_COMMAND   0x00
#define MESSAGE_TYPE_NOTIFICATION    0x01
#define MESSAGE_TYPE_NOTIFICATION_REQUEST	0x02
#define MESSAGE_TYPE_SUBSCRIBE_REQUEST	0x03
#define MESSAGE_TYPE_RESPONSE	0x04
#define MESSAGE_TYPE_INTEREST_NOTIFICATION	0x05
#define MESSAGE_TYPE_CAPABILITY_NOTIFICATION	0x06
#define MESSAGE_TYPE_SUBSCRIBE_INSTRUCTION	0x07
#define MESSAGE_TYPE_SAFETY_ALARM	0x08
#define MESSAGE_TYPE_RAW_DATA	0x09

// Data type defines
#define UNIT_TYPE_POWERED    0x00
#define UNIT_TYPE_DEGREES_C_ENVIRONMENT	0x01
#define UNIT_TYPE_DEGREES_C_DEVICE	0x02
#define UNIT_TYPE_WATTS	0x03
#define UNIT_TYPE_LUMENS	0x04
#define UNIT_TYPE_OCCUPANTS	0x05
#define UNIT_TYPE_PRESSURE	0x06
#define UNIT_TYPE_DIMMING_LEVEL	0x07
#define UNIT_TYPE_OPERATING_MODE	0x08
#define UNIT_TYPE_LOCALE	0x09


// Define user commands
#define USER_COMMAND_CHANGE_VALUE    0x00
#define USER_COMMAND_PROMPT_ABILITIES	0x01


typedef struct 
{
    uint16 sourceNetworkAddress;
    uint32 macHi;
    uint32 macLow;
    uint8 sourceLocale;
    uint16 sourceDeviceType;
    uint8 count;
    uint8 index;
    uint8 type;
    uint8 length;
    uint8 *data;
    
} message;

typedef struct
{
	uint8 command;
	int32 data;
	uint8 type;
	
} userCommand;

typedef struct
{
    uint8 numNotifications;
    uint8 *types;
    int32 *data;
    
} notification;

typedef struct
{
	uint8 numTypes;
	uint8 *types;

} notificationRequest;

typedef struct
{
	uint8 numTypes;
	uint8 *types;

} subscribeRequest;

typedef struct
{
	uint8 something; // soooooooo TBD
	
} response;

typedef struct
{
	uint8 numCapabilities;
	uint8 *capabilities;
	
} capabilitiesNotification;

typedef struct
{
	uint8 numInterests;
	uint8 *interests;
	uint8 *locales;
	
} interestsNotification;

typedef struct
{
	uint16 networkAddress;
	uint32 macLow;
	uint32 macHi;
	uint8 locale;
	uint8 numCapabilities;
	uint8 *capabilities;

} subscribeInstruction;

typedef struct
{
	uint8 riskType;
	uint8 riskLocale;
	
} safetyAlarm;

typedef struct
{
	uint8 numBytes;
	uint8 *data;
	
} rawData;