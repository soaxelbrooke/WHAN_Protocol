

#include "whan_protocol.h"

// This function is for interfacing with the UART interface
void sendMessage(uint32 macHi, uint32 macLow, uint16 networkAddress, 
    uint8 rfDataLength, uint8 *rfData)
{
    
    // TODO: fill in this function to tie it to the system UART
    // It should implement the Zigbee Send Request command
    
}

// Get message from the receive buffer and put it into the passed in variables
void getMessage(uint32 *macHi, uint32 *macLow, uint16 *networkAddress,
    uint8 *rfData)
{
    
    // TODO: fill in this function to tie it to the system UART
    // It should implement the Zigbee Receive Packet command
    
}

// Get message length (the length of rfData)
uint8 getMessageLength(void)
{
    
    // TODO: fill in this function to allow host processor to allocate
    // proper amount of memory for getMessage function.
    
}

// This function is called by the top level functions, and integrates with the
// lower level sendMessage() function
void sendFormattedMessage(uint16 networkAddress, message myMessage)
{
    uint8 i = 0;
    uint32 macLow = getMacLow(networkAddress);
    uint32 macHigh = getMacHigh(networkAddress);
    uint8 rfDataLength = myMessage.messageLength + 7;
    uint8 messageBuffer[rfDataLength];
    
    messageBuffer[0] = myMessage.sourceLocale;
    messageBuffer[1] = (uint8)(myMessage.sourceDeviceType);
    messageBuffer[2] = (uint8)(myMessage.sourceDeviceType >> 8);
    messageBuffer[3] = myMessage.messageCount;
    messageBuffer[4] = myMessage.messageIndex;
    messageBuffer[5] = myMessage.messageType;
    messageBuffer[6] = myMessage.messageLength;
    
    for (i = 0; i < messageLength; i++) 
    {
        messageBuffer[i+7] = myMessage.messageData[i];
        
    }
    
    sendMessage(macHigh, macLow, networkAddress, rfDataLength, messageBuffer);
    
}

// Gets the message type, allowing for branching based on what was received
uint8 getMessageType(uint8 *messageData)
{
    return messageData[5];
    
}

// User Commands
void sendUserCommandChangeValue(uint16 networkAddr, uint8 targetValueType, 
    uint32 targetValueData)
{
    message myMessage;
    uint8 buffer[6];
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_USER_COMMAND;
    myMessage.messageLength = 6;
    myMessage.messageData = buffer;
    
    // Populate data buffer
    buffer[0] = USER_COMMAND_CHANGE_VALUE;
    buffer[1] = targetValueType;
    buffer[2] = (uint8) ()targetValueData >> 24); // Big endian transmission
    buffer[3] = (uint8) ()targetValueData >> 16);
    buffer[4] = (uint8) ()targetValueData >> 8);
    buffer[5] = (uint8) ()targetValueData >> 0);
    
    sendFormattedMessage(targetAddr, myMessage);
    
}
    
void sendUserCommandPromptAbilities(uint16 networkAddress)
{
    message myMessage;
    uint8 buffer[1];
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_USER_COMMAND;
    myMessage.messageLength = 1;
    myMessage.messageData = buffer;
    
    buffer[0] = USER_COMMAND_PROMPT_ABILITIES;
    
    sendFormattedMessage(networkAddress, myMessage);
    
}


// Notifications (general)
void sendNotification(uint16 networkAddress, notification myNotification)
{
    message myMessage;
    uint8 bufferLen = myNotification.numNotifications*5; // This will need to change for multiple messages...
    uint8 i = 0;
    uint8 buffer[bufferLen];
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;//TODO: CALCULATE RQUIRED NUM OF MESSAGES!
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_NOTIFICATION;
    myMessage.messageLength = bufferLen;
    myMessage.messageData = buffer;
    
    buffer[0] = myNotification.numNotifications;
    for (i = 0; i < myNotification.numNotifications; i++)
    {
        buffer[i*5+1] = (uint8) (myNotification.data[i] >> 24);
        buffer[i*5+2] = (uint8) (myNotification.data[i] >> 16);
        buffer[i*5+3] = (uint8) (myNotification.data[i] >> 8);
        buffer[i*5+4] = (uint8) (myNotification.data[i] >> 0);
        
        buffer[i*5+1] = myNotification.type[i];
        
    }
    
    sendFormattedMessage(networkAddress, myMessage);
    
}

void sendNotifications(uint8 numberOfDestinations, uint16 *networkAddresses, 
    notification myNotification)
{
    message myMessage;
    uint8 bufferLen = myNotification.numNotifications*5; // This will need to change for multiple messages...
    uint8 i = 0;
    uint8 buffer[bufferLen];
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;//TODO: CALCULATE RQUIRED NUM OF MESSAGES!
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_NOTIFICATION;
    myMessage.messageLength = bufferLen;
    myMessage.messageData = buffer;
    
    // Populate buffer
    buffer[0] = myNotification.numNotifications;
    for (i = 0; i < myNotification.numNotifications; i++)
    {
        buffer[i*5+1] = (uint8) (myNotification.data[i] >> 24);
        buffer[i*5+2] = (uint8) (myNotification.data[i] >> 16);
        buffer[i*5+3] = (uint8) (myNotification.data[i] >> 8);
        buffer[i*5+4] = (uint8) (myNotification.data[i] >> 0);
        
        buffer[i*5+1] = myNotification.type[i];
        
    }
    
    for (i = 0; i < numberOfDestinations; i++)
    {
        sendFormattedMessage(networkAddresses[i], myMessage);
        
    }
    
}


// Notification Request
void sendNotificationRequest(uint16 networkAddress)
{
    message myMessage;
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_NOTIFICATION_REQUEST;
    myMessage.messageLength = 0;
    
    sendFormattedMessage(networkAddress, myMessage);
    
}


// Subscribe Request
void sendSubscribeRequest(uint16 networkAddress)
{
    message myMessage;
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_SUBSCRIBE_REQUEST;
    myMessage.messageLength = 0;
    
    sendFormattedMessage(networkAddress, myMessage);
    
}


// Capability/Interest Notifications
void sendCapabilitiesNotification(uint8 capabilitiesLength, 
    uint8 *capabilities)
{
    message myMessage;
    uint8 buffer[capabilitiesLength];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_CAPABILITY_NOTIFICATION;
    myMessage.length = 0;
    myMessage.data = buffer;
    
    buffer[0] = capabilitiesLength;
    for (i = 0; i < capabilitiesLength; i++)
    {
        buffer[i+1] = capabilities[i];
        
    }
    
    sendFormattedMessage(0, myMessage);
    
}

void sendInterestsNotification(uint8 interestsLength, uint8 *interests,
    uint8 *locales)
{ // TODO: This will definitely need multi-spanning messages.
    message myMessage;
    uint8 buffer[2*interestsLength];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_INTEREST_NOTIFICATION;
    myMessage.length = 0;
    myMessage.data = buffer;
    
    buffer[0] = interestsLength;
    for (i = 0; i < interestsLength; i++)
    {
        buffer[2*i+1] = interests[i];
        buffer[2*i+2] = locales[i];
    }
    
    sendFormattedMessage(0, myMessage);
    
}

#IFDEF COORDINATOR
// Subscribe Instruction
void sendSubscribeInstruction(uint16 targetNetworkAddress, 
    uint16 networkAddress, uint32 macHi, uint32 macLow, uint8 locale, 
    uint8 numCapabilities, uint8 *capabilities)
{
    message myMessage;
    uint8 buffer[12 + numCapabilites];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_SUBSCRIBE_INSTRUCTION;
    myMessage.messageLength = 12 + numCapabilities;
    myMessage.messageData = buffer;
    
    buffer[0] = (uint8)(networkAddress >> 8);
    buffer[1] = (uint8)(networkAddres);
    buffer[2] = (uint8)(macHi >> 24);
    buffer[3] = (uint8)(macHi >> 16);
    buffer[4] = (uint8)(macHi >> 8);
    buffer[5] = (uint8)(macHi >> 0);
    buffer[6] = (uint8)(macLow >> 24);
    buffer[7] = (uint8)(macLow >> 16);
    buffer[8] = (uint8)(macLow >> 8);
    buffer[9] = (uint8)(macLow >> 0);
    buffer[10] = locale;
    buffer[11] = numCapabilities;
    for (i = 0; i < numCapabilities; i++)
    {
        buffer[i+12] = capabilities[i];
    }
    
    sendFormattedMessage(targetNetworkAddress, myMessage);
}

#ENDIF

// Safety Alarms
void sendSafetyAlarm(uint8 riskType, uint8 riskLocale)
{
    // TODO: add something for possible universale risk locale? (0?)
    message myMessage;
    
    getNodesInLocale(riskLocale, nodesInLocale);
    
    // Populate message fields
    myMessage.sourceLocale = myLocale();
    myMessage.deviceType = MY_DEVICE_TYPE;
    myMessage.messageCount = 0;
    myMessage.messageIndex = 0;
    myMessage.messageType = MESSAGE_TYPE_SAFETY_ALARM;
    myMessage.messageLength = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale == riskLocale)
        {
            sendFormattedMessage(addressBook[i].networkAddress, myMessage);
            
        }
        
    }
    
    // Send safety alert to coordinator for good measure
    sendFormattedMessage(0, myMessage);
    
}

// Scans the address book for nodes that are interested in data contained in the
// passed in notification and sends data to these nodes.  Returns the number of
// nodes notified.
uint8 notifyInterestedNodes(notification myNotification)
{
    uint8 i = 0, j = 0, k = 0, numNotified = 0;
    notification N;
    int32 data[myNotification.numNotifications];
    int8 types[myNotification.numNotifications];
    N.data = data;
    N.types = types;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale != 0)
        {
            // For each entry in the address book, construct a notification 
            // containing data that entry is interested in
            N.numNotifications = 0;
            
            for (j = 0; j < myNotification.numNotifications; j++)
            {
                // For each notification in the passed in notification, check
                // if this entry is interested in it
                
                for (k = 0; k < addressBook[i].numInterests; k++)
                {
                    // For each interest in this entry, check if the passed in 
                    // notification is equal to it
                    
                    if (addressBook[i].interests[k] == myNotification.types[j])
                    {
                        // If entry is interested, add this data to the notification
                        N.data[N.numNotifications] = myNotification.data[j];
                        N.types[N.numNotifications] = myNotification.types[j];
                        N.numNotifications += 1;
                        
                        // Exit k for loop (this data type has been handled)
                        break;
                        
                    }
                    
                }
                
            }
            
        }
        
        // Check if this entry is interested in anything from the notification
        if (N.numNotifications > 0)
        {
            // Send constructed notification
            sendNotificaiton(addressBook[i].networkAddress, N);
            numNotified += 1;
            
        }
        
    }
    
    return numNotified;
    
}