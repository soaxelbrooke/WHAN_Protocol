
#include <whan_protocol.h>

static uint8 myCapabilities[] = {UNIT_TYPE_DEGREES_C_ENVIRONMENT,
    UNIT_TYPE_PRESSURE, UNIT_TYPE_LUMENS};
static uint8 myInterests[] = {UNIT_TYPE_OCCUPANTS};


// Function pointers
void serviceSubscribeRequest(uint16 networkAddress, uint32 macHi, uint32 macLow,
    message *myMessage);
void serviceSubscribeInstruction(uint16 networkAddress, uint32 macHi, 
    uint32 macLow, message *myMessage);
void serviceNotificationRequest(uint16 networkAddress, message *myMessage);
int32 getLastMeasurement(uint8 type);
void sleepForTenMinutes(void);
void init(void);
int32 measureTemp(void);
int32 measurePressure(void);
int32 measureLumens(void);

// Main loop
void main(void)
{
    // Initialize variables for measurements
    int32 temperature, pressure, light;
    // Initialize notification struct
    notification myNotification;
    
    // Point struct to lists in memory
    int32 notifData[3];
    int8 notifTypes[3];
    myNotification.data = notifData;
    myNotification.types = notifTypes;
    // Specify number of notifications
    myNotification.numNotifications = 3;
    
    // Initialize some stuff
    init();
    
    // Send measurement capabilities to coordinator
    sendCapabilitiesNotification(MY_NUM_CAPABILITIES, myCapabilities);
    
    
    // Send measurement interests to cooridnator
    sendInterestsNotification(MY_NUM_INTERESTS, myInterests, uint8 *locales);
    
    while(1)
    {
        // Measure temperature
        temperature = measureTemp();
        // Put measured temp data into struct
        myNotification.data[0] = temperature;
        myNotification.types[0] = UNIT_TYPE_DEGREES_C_ENVIRONMENT;
        
        // Measure pressure
        pressure = measurePressure();
        // Put measured pressure data into struct
        myNotificaiton.data[1] = pressure;
        myNotification.types[1] = UNIT_TYPE_PRESSURE;
        
        // Measure light
        light = measureLight();
        // Put measured light data into struct
        myNotification.data[2] = light;
        myNotification.types[2] = UNIT_TYPE_LUMENS;
        
        // Send notification data to address book entries that are interested
        notifyInterestedNodes(myNotification);
        
        // Wait for a while
        sleepForTenMinutes();
        
    }
    
}

// Should be called when a Subscribe Request has been received
void serviceSubscribeRequest(uint16 networkAddress, uint32 macHi, uint32 macLow,
    message *myMessage)
{
    // Add node to address book (if there is space)
    if (numFreeAddressEntries() > 0)
    {
        addNodeToAddressBook(networkAddress, macHi, macLow, 
            *myMessage.sourceLocale, *myMessage.data[0],
            (uint8*)(*myMessage.data + 1), 0, myCapabilities);
            
    }
    
}

// Should be called when a Subscribe Instruction has been received
void serviceSubscribeInstruction(uint16 networkAddress, uint32 macHi, 
    uint32 macLow, message *myMessage)
{
    // Get data needed to add node to address book
    uint16 targetNetworkAddress = *myMessage.data[1] + 
        (*myMessage.data[0] << 8);
    uint32 targetMacHi = *myMessage.data[5] + 
        (*myMessage.data[4] << 8) + 
        (*myMessage.data[3] << 16) + 
        (*myMessage.data[2] << 24);
    uint32 targetMacLow = *myMessage.data[9] + 
        (*myMessage.data[8] << 8) + 
        (*myMessage.data[7] << 16) + 
        (*myMessage.data[6] << 24);
    uint8 targetLocale = *myMessage.data[10];
    uint8 targetNumCapabilities = *myMessage.data[11];
    uint8 *targetCapabilities = (uint8*)(*myMessage.data + 12);
    
    // Add node to address book (if there is space)
    if (numFreeAddressEntries() > 0)
    {
        addNodeToAddressBook(targetNetworkAddress, targetMacHi, targetMacLow,
            targetLocale, 0, myInterests, targetNumCapabilities,
            targetCapabilities);
    }
    
    // Request subscription from target node
    sendSubscribeRequest(targetNetworkAddress, MY_NUM_INTERESTS, myInterests);
    
}

// Should be called when a Notification Request command has been received
void serviceNotificationRequest(uint16 networkAddress, message *myMessage)
{
    // Declare variables to be used in this context
    uint8 i = 0, j = 0;
    notificaiton N;
    uint32 nData[MY_NUM_CAPABILITIES];
    uint8 nTypes[MY_NUM_CAPABILITIES];
    
    // Set up struct pointers to lists on stack
    N.numNotifications = 0;
    N.data = nData;
    N.types = nTypes;
    
    // For all types that are in both the notification request and this node's
    // list of capabilities, append their latest measurements to the
    // notification
    for (i = 0; i < MY_NUM_CAPABILITIES; i++)
    {
        for (j = 0; j < *myMessage.data[0]; j++)
        {
            if (myCapabilities[i] == *myMessage.data[j+1])
            {
                // If one of this node's capabilities are found in the
                // notification request, add to the notification
                N.data[N.numNotifications] = getLastMeasurement(*myMessage.data[j+1]);
                N.types[N.numNotifications] = *myMessage.data[j+1];
                N.numNotifications += 1;
                
            }
            
        }
        
    }
    
    // If there are notifications to be sent, send them
    if (N.numNotifications > 0)
    {
        sendNotification(uint16 networkAddress, N);
        
    }
    
}

// Retrieves the last measurement of a given type
int32 getLastMeasurement(uint8 type)
{
    int32 result = 0;
    
    if (type == UNIT_TYPE_DEGREES_C_ENVIRONMENT)
    {
        result = measureTemp();
    }
    else if (type == UNIT_TYPE_PRESSURE)
    {
        result = measurePressure();
    } 
    else if (type == UNIT_TYPE_LUMENS)
    {
        result = measureLumens();
    }
    
    return result;
    
}

// Sleep for ten minutes
void sleepForTenMinutes(void)
{
    return; // It's a very fast ten minutes.
}

void init(void)
{
    return; // Not much to initialize, I suppose...
}

int32 measureTemp(void)
{
    return 0x00000001;
}

int32 measurePressure(void)
{
    return 0x00000002;
}

int32 measureLumens(void)
{
    return 0x00000003;
}
