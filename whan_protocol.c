

#include "whan_protocol.h"


// This function is for interfacing with the UART interface
void sendMessage(uint32 macHi, uint32 macLow, uint16 networkAddress, 
    uint8 rfDataLength, uint8 *rfData)
{
    
    // TODO: fill in this function to tie it to the system UART
    // It should implement the Zigbee Send Request command
    Zigbee_UART_1_SendMessage(macHi, macLow, networkAddress, rfDataLength, 
        rfData);
	
}

// Get message from the receive buffer and put it into the passed in variables
void getMessage(uint32 *macHi, uint32 *macLow, uint16 *networkAddress,
    uint8 *rfData)
{
    
    // TODO: fill in this function to tie it to the system UART
    // It should implement the Zigbee Receive Packet command
    Zigbee_UART_1_GetMessage(macHi, macLow, networkAddress, rfData);
	
	
}

// Populates the passed in struct with data from the RX buffer
// TODO: this is the second of three buffers if this command is used.  This is
// probably a good target for optimization.
void getMessageStruct(uint16 *srcNetworkAddress, uint32 *srcMacLow, 
    uint32 *srcMacHi, message *myMessage)
{
	uint8 rfData[128];
	
	getMessage(srcMacHi, srcMacLow, srcNetworkAddress, rfData);
	
	myMessage->sourceLocale = rfData[0];
	myMessage->sourceDeviceType = (uint16) rfData[1];
	myMessage->count = rfData[3];
	myMessage->index = rfData[4];
	myMessage->type = rfData[5];
	myMessage->length = rfData[6];
	memcpy(myMessage->data, (uint8 *)(rfData + 7), myMessage->length);
	
}

// Get message length (the length of rfData)
uint8 getMessageLength(void)
{
    
    // TODO: fill in this function to allow host processor to allocate
    // proper amount of memory for getMessage function.
	return Zigbee_UART_1_GetMessageLength();
	
}

// This function is called by the top level functions, and integrates with the
// lower level sendMessage() function
void sendFormattedMessage(uint16 networkAddress, message myMessage)
{
    uint8 i = 0;
    uint32 macLow = getMacLow(networkAddress);
    uint32 macHigh = getMacHigh(networkAddress);
    uint8 rfDataLength = myMessage.length + 7;
    uint8 messageBuffer[128];
    
    messageBuffer[0] = myMessage.sourceLocale;
    messageBuffer[1] = (uint8)(myMessage.sourceDeviceType);
    messageBuffer[2] = (uint8)(myMessage.sourceDeviceType >> 8);
    messageBuffer[3] = myMessage.count;
    messageBuffer[4] = myMessage.index;
    messageBuffer[5] = myMessage.type;
    messageBuffer[6] = myMessage.length;
    
    for (i = 0; i < myMessage.length; i++) 
    {
        messageBuffer[i+7] = myMessage.data[i];
        
    }
    
    sendMessage(macHigh, macLow, networkAddress, rfDataLength, messageBuffer);
    
}

// Gets the message type, allowing for branching based on what was received
uint8 getMessageType(uint8 *messageData)
{
    return messageData[5];
    
}

// User Commands
void sendUserCommandChangeValue(uint16 networkAddress, uint8 targetValueType, 
    uint32 targetValueData)
{
    message myMessage;
    uint8 buffer[6];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_USER_COMMAND;
    myMessage.length = 6;
    myMessage.data = buffer;
    
    // Populate data buffer
    buffer[0] = USER_COMMAND_CHANGE_VALUE;
    buffer[1] = targetValueType;
    buffer[2] = (uint8) (targetValueData >> 24); // Big endian transmission
    buffer[3] = (uint8) (targetValueData >> 16);
    buffer[4] = (uint8) (targetValueData >> 8);
    buffer[5] = (uint8) (targetValueData >> 0);
    
    sendFormattedMessage(networkAddress, myMessage);
    
}
    
void sendUserCommandPromptAbilities(uint16 networkAddress)
{
    message myMessage;
    uint8 buffer[1];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_USER_COMMAND;
    myMessage.length = 1;
    myMessage.data = buffer;
    
    buffer[0] = USER_COMMAND_PROMPT_ABILITIES;
    
    sendFormattedMessage(networkAddress, myMessage);
    
}


// Notifications (general)
void sendNotification(uint16 networkAddress, notification myNotification)
{
    message myMessage;
    uint8 bufferLen = myNotification.numNotifications*5; // This will need to change for multiple messages...
    uint8 i = 0;
    uint8 buffer[5*MY_NUM_CAPABILITIES+1];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;//TODO: CALCULATE RQUIRED NUM OF MESSAGES!
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_NOTIFICATION;
    myMessage.length = bufferLen;
    myMessage.data = buffer;
    
    buffer[0] = myNotification.numNotifications;
    for (i = 0; i < myNotification.numNotifications; i++)
    {
        buffer[i*5+1] = (uint8) (myNotification.data[i] >> 24);
        buffer[i*5+2] = (uint8) (myNotification.data[i] >> 16);
        buffer[i*5+3] = (uint8) (myNotification.data[i] >> 8);
        buffer[i*5+4] = (uint8) (myNotification.data[i] >> 0);
        
        buffer[i*5+5] = myNotification.types[i];
        
    }
    
    sendFormattedMessage(networkAddress, myMessage);
    
}

void sendNotifications(uint8 numberOfDestinations, uint16 *networkAddresses, 
    notification myNotification)
{
    message myMessage;
    uint8 bufferLen = myNotification.numNotifications*5; // This will need to change for multiple messages...
    uint8 i = 0;
    uint8 buffer[5*MY_NUM_CAPABILITIES+1];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;//TODO: CALCULATE RQUIRED NUM OF MESSAGES!
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_NOTIFICATION;
    myMessage.length = bufferLen;
    myMessage.data = buffer;
    
    // Populate buffer
    buffer[0] = myNotification.numNotifications;
    for (i = 0; i < myNotification.numNotifications; i++)
    {
        buffer[i*5+1] = (uint8) (myNotification.data[i] >> 24);
        buffer[i*5+2] = (uint8) (myNotification.data[i] >> 16);
        buffer[i*5+3] = (uint8) (myNotification.data[i] >> 8);
        buffer[i*5+4] = (uint8) (myNotification.data[i] >> 0);
        
        buffer[i*5+5] = myNotification.types[i];
        
    }
    
    for (i = 0; i < numberOfDestinations; i++)
    {
        sendFormattedMessage(networkAddresses[i], myMessage);
        
    }
    
}


// Notification Request
void sendNotificationRequest(uint16 networkAddress, uint8 numTypes, 
    uint8 *types)
{
    uint8 i = 0;
    message myMessage;
    uint8 buffer[MY_NUM_INTERESTS+1];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_NOTIFICATION_REQUEST;
    myMessage.length = 0;
    
    buffer[0] = numTypes;
    for (i = 0; i < numTypes; i++)
    {
        buffer[i+1] = types[i];
        
    }
    
    sendFormattedMessage(networkAddress, myMessage);
    
}


// Subscribe Request
void sendSubscribeRequest(uint16 networkAddress, uint8 numTypes,
    uint8 *types)
{
    uint8 i = 0;
    message myMessage;
    uint8 buffer[MY_NUM_INTERESTS+1];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_SUBSCRIBE_REQUEST;
    myMessage.length = 0;
    
    buffer[0] = numTypes;
    for (i = 0; i < numTypes; i++)
    {
        buffer[i+1] = types[i];
        
    }
    
    sendFormattedMessage(networkAddress, myMessage);
    
}

// Send Capability Notifications
void sendMyCapabilitiesNotification(void)
{
	sendCapabilitiesNotification(MY_NUM_CAPABILITIES, getMyCapabilitiesPtr());
	
}

void sendCapabilitiesNotificationStruct(
	capabilitiesNotification *myCapabilitiesNotification)
{
	sendCapabilitiesNotification(myCapabilitiesNotification->numCapabilities,
		myCapabilitiesNotification->capabilities);
	
}

void sendCapabilitiesNotification(uint8 capabilitiesLength, 
    uint8 *capabilities)
{
    message myMessage;
    uint8 buffer[MY_NUM_CAPABILITIES+1];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
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

// Send Interest Notifications
void sendMyInterestsNotification(void)
{
	sendInterestsNotification(MY_NUM_INTERESTS, getMyInterestsPtr(), 
		getMyInterestsLocalesPtr());
	
}

void sendInterestsNotificationStruct(
	interestsNotification *myInterestsNotification)
{
	sendInterestsNotification(myInterestsNotification->numInterests,
		myInterestsNotification->interests, myInterestsNotification->locales);
	
}

void sendInterestsNotification(uint8 interestsLength, uint8 *interests,
    uint8 *locales)
{ // TODO: This will definitely need multi-spanning messages.
    message myMessage;
    uint8 buffer[2*MY_NUM_INTERESTS+1];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
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

#ifdef COORDINATOR
// Subscribe Instruction
void sendSubscribeInstruction(uint16 targetNetworkAddress, 
    uint16 networkAddress, uint32 macHi, uint32 macLow, uint8 locale, 
    uint8 numCapabilities, uint8 *capabilities)
{
    message myMessage;
    uint8 buffer[12 + MY_NUM_CAPABILITIES];
    uint8 i = 0;
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_SUBSCRIBE_INSTRUCTION;
    myMessage.length = 12 + numCapabilities;
    myMessage.data = buffer;
    
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

#endif

// Safety Alarms
void sendSafetyAlarm(uint8 riskType, uint8 riskLocale)
{
    // TODO: add something for possible universale risk locale? (0?)
    uint8 i = 0;
    message myMessage;
    uint8 numNodes = numNodesInLocale(riskLocale);
    uint16 nodeList[MY_ADDRESS_BOOK_LENGTH];
    
    nodesInLocale(nodeList, riskLocale);
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_SAFETY_ALARM;
    myMessage.length = 0;
    
    for (i = 0; i < numNodes; i++)
    {
        sendFormattedMessage(nodeList[i], myMessage);
        
    }
    
    // Send safety alert to coordinator for good measure
    sendFormattedMessage(0, myMessage);
    
}

// Send a raw packet of data.
void sendRawData(uint16 networkAddress, uint8 rawDataLength, uint8 *rawData)
{
    message myMessage;
    uint8 buffer[MAX_SINGLE_MESSAGE_LENGTH];
    
    // Populate message fields
    myMessage.sourceLocale = getMyLocale();
    myMessage.sourceDeviceType = MY_DEVICE_TYPE;
    myMessage.count = 0;
    myMessage.index = 0;
    myMessage.type = MESSAGE_TYPE_RAW_DATA;
    myMessage.length = rawDataLength;
    myMessage.data = buffer;
    
    memcpy(buffer, rawData, rawDataLength);
    
    sendFormattedMessage(networkAddress, myMessage);
    
}

// Get notification data from message buffer
void getNotification(notification *myNotification, message myMessage)
{
    uint8 i = 0;
    
    for (i = 0; i < myNotification->numNotifications; i++)
    {
        // TODO: figure out how to do this as a casted pointer instead of shifting and adding
        // TODO: perhaps the notification data should really be data1 data2 data3 type1 type2 type3?
        myNotification->data[i] = (int32)((myMessage.data[5*i+1] << 0) + 
            (myMessage.data[5*i+2] << 8) + (myMessage.data[5*i+3] << 16) + 
            (myMessage.data[5*i+4] << 24));
        myNotification->types[i] = (uint8)(myMessage.data[5*i+5]);
    }
    
}
    
// Determine how many notifications are in the message currently in the message 
// buffer
uint8 getNumNotifications(message myMessage)
{
    uint8 result = 0;
    
    result = myMessage.data[0];
    
    return result;
    
}

// Get usercommand from the message buffer, populate passed in struct
void getUserCommand(userCommand *myUserCommand, message myMessage)
{
	myUserCommand->command = myMessage.data[0];
	
	if (myUserCommand->command == USER_COMMAND_CHANGE_VALUE)
	{
		myUserCommand->data = (int32) myMessage.data[1]; // TODO: does this work like I think it does?
		myUserCommand->type = myMessage.data[5];
		
	}
	
}

// Get notification request from the message buffer
void getNotificationRequest(notificationRequest *myNotificationRequest, 
	message myMessage)
{
	myNotificationRequest->numTypes = myMessage.data[0];
	memcpy(myNotificationRequest->types, (uint8*) (myMessage.data + 1), 
		myNotificationRequest->numTypes);
	
}

// Get subscribe request from message buffer
void getSubscribeRequest(subscribeRequest *mySubscribeRequest, 
	message myMessage)
{
	mySubscribeRequest->numTypes = myMessage.data[0];
	memcpy(mySubscribeRequest->types, (uint8*) (myMessage.data + 1), 
		mySubscribeRequest->numTypes);
	
}

// Get response from the message buffer
void getResponse(response *myResponse, message myMessage)
{
	// TBD
}

// Get capabilitiy notification from the message buffer
void getCapabilityNotifification(
	capabilitiesNotification *myCapabilitiesNotification, message myMessage)
{
	myCapabilitiesNotification->numCapabilities = myMessage.data[0];
	memcpy(myCapabilitiesNotification->capabilities, 
		(uint8*) (myMessage.data + 1), 
		myCapabilitiesNotification->numCapabilities);
	
}
	
// Get interest notification from the message buffer
void getInterestsNotification(interestsNotification *myInterestsNotification,
	message myMessage)
{
	uint8 i = 0;
	
	myInterestsNotification->numInterests = myMessage.data[0];
	for (i = 0; i < myInterestsNotification->numInterests; i++)
	{
		myInterestsNotification->interests[i] = myMessage.data[2*i+1];
		myInterestsNotification->locales[i] = myMessage.data[2*i+2];
		
	}
	
}
	
// Get subscribe instruction from the message buffer
void getSubscribeInstruction(subscribeInstruction *mySubscribeInstruction,
	message myMessage)
{
	mySubscribeInstruction->networkAddress = (uint16) myMessage.data[0]; // TODO: does this work like I think it does?
	mySubscribeInstruction->macLow = (uint32) myMessage.data[2];
	mySubscribeInstruction->macHi = (uint32) myMessage.data[6];
	mySubscribeInstruction->locale = myMessage.data[10];
	mySubscribeInstruction->numCapabilities = myMessage.data[11];
	memcpy(mySubscribeInstruction->capabilities, (uint8*) (myMessage.data + 12), 
		mySubscribeInstruction->numCapabilities);
	
}
	
// Get safety alarm form the message buffer
void getSafetyAlarm(safetyAlarm *mySafetyAlarm, message myMessage)
{
	mySafetyAlarm->riskType = myMessage.data[0];
	mySafetyAlarm->riskLocale = myMessage.data[1];
	
}

// Get raw data from the message buffer
void getRawData(rawData *myRawData, message myMessage)
{
	myRawData->numBytes = myMessage.data[0];
	memcpy(myRawData->data, (uint8*) (myMessage.data + 1), 
		myRawData->numBytes);
	
}

// Scans the address book for nodes that are interested in data contained in the
// passed in notification and sends data to these nodes.  Returns the number of
// nodes notified.
uint8 notifyInterestedNodes(notification myNotification)
{
    uint8 i = 0, j = 0, k = 0, numNotified = 0;
    notification N;
    int32 data[MY_NUM_CAPABILITIES];
    uint8 types[MY_NUM_CAPABILITIES];
    
/*    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
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
        
    }*/
    
    uint8 numNodes = numAddressEntries();
    uint16 nodeList[MY_ADDRESS_BOOK_LENGTH];
    uint8 typeTemp[MY_NUM_CAPABILITIES];
    
    N.data = data;
    N.types = types;
    
    nodesInAddressBook(nodeList);
    
    // Construct a special notification for each node in the address book
    for (i = 0; i < numNodes; i++)
    {
        //N.numNotifications = numNodeInterests(nodeList[i]);
        nodeInterests(typeTemp, nodeList[i]);
        N.numNotifications = 0;
        
        // For each notification type in the passed in notification, check
        // if this entry is interested in it
        
        for (j = 0; j < myNotification.numNotifications; j++)
        {
            // For each interest in this entry, check if the passed in 
            // notification is equal to it
            
            for (k = 0; k < numNodeInterests(nodeList[i]); k++)
            {
                if (typeTemp[k] == myNotification.types[j])
                {
                    // If entry is interested, add this data to notification
                    N.data[N.numNotifications] = myNotification.data[j];
                    N.types[N.numNotifications] = myNotification.types[j];
                    N.numNotifications += 1;
                    
                    // Exit k for loop, as this data type has been handled
                    break;
                    
                }
                
            }
            
        }
        
        
        // Send notification if it has any interests in the new data
        if (N.numNotifications > 0)
        {
            sendNotification(nodeList[i], N);
            numNotified += 1;
            
        }
        
    }
    
    
    return numNotified;
    
}
