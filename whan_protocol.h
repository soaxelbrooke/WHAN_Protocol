// WHAN Protocol header file
// By Stuart Owen
//
// Change Log:
//  11/8/2012   - Created

#include "address_book.h"

// Send a message via the Zigbee Transmit Request command
// See pg 89 of ftp://ftp1.digi.com/support/documentation/90000976_C.pdf
void sendMessage(uint32 macHi, uint32 macLow, uint16 networkAddress, 
    uint8 rfDataLength, uint8 *rfData);

// Get message from the receive buffer and put it into the passed in variables
void getMessage(uint32 *macHi, uint32 *macLow, uint16 *networkAddress,
    uint8 *rfData);

// Populates the passed in struct with data from the RX buffer
void getMessageStruct(uint16 *srcNetworkAddress, uint32 *srcMacLow, 
    uint32 *srcMacHi, message *myMessage);

// Get message length (number of bytes in the "Received Data" field in the 
// Zigbee Receive Packet frame)
// See pg 96 of ftp://ftp1.digi.com/support/documentation/90000976_C.pdf
uint8 getMessageLength(void);
    
// This function is called by the top level functions, and integrates with the
// lower level sendMessage() function
void sendFormattedMessage(uint16 networkAddress, message myMessage);

// Gets the message type, allowing for branching based on what was received
uint8 getMessageType(uint8 *messageData);

// User Commands
void sendUserCommandChangeValue(uint16 networkAddr, uint8 targetValueAddr, 
    uint32 targetValueData);
void sendUserCommandPromptAbilities(uint16 networkAddr);

// Notifications (general)
void sendNotification(uint16 networkAddress, notification myNotification);
void sendNotifications(uint8 numberOfDestinations, uint16 *networkAddresses, 
    notification myNotification);

// Notification Request
void sendNotificationRequest(uint16 networkAddress, uint8 numTypes, 
    uint8 *types);

// Subscribe Request
void sendSubscribeRequest(uint16 networkAddress, uint8 numTypes,
    uint8 *types);

#ifdef COORDINATOR
// Subscribe Instruction
void sendSubscribeInstruction(uint16 targetNetworkAddress, 
    uint16 networkAddress, uint32 macHi, uint32 macLow, uint8 locale, 
    uint8 numberOfCapabilities, uint8 *capabilities);
#endif

// Response
// TODO: better define this!
void sendResponse(uint16 networkAddress);

// Capability/Interest Notifications
void sendMyCapabilitiesNotification(void);
void sendCapabilitiesNotification(uint8 capabilitiesLength, 
    uint8 *capabilities);
void sendCapabilitiesNotificationStruct(
	capabilitiesNotification *myCapabilitiesNotificaiton);
	
void sendMyInterestsNotification(void);
void sendInterestsNotification(uint8 interestsLength, uint8 *interests,
    uint8 *locales);
void sendInterestsNotificationStruct(interestsNotification 
	*myInterestsNotification);

// Safety Alarms
void sendSafetyAlarm(uint8 riskType, uint8 riskLocale);

// Raw Data
void sendRawData(uint16 networkAddress, uint8 dataLength, uint8 *data);

// Get notification data from message buffer
void getNotification(notification *myNotification, message myMessage);
    
// Determine how many notifications are in the message currently in the message 
// buffer
uint8 getNumNotifications(message myMessage);

// Get usercommand from the message buffer, populate passed in struct
void getUserCommand(userCommand *myUserCommand, message myMessage);

// Get notification request from the message buffer
void getNotificationRequest(notificationRequest *myNotificationRequest, 
	message myMessage);

// Get subscribe request from message buffer
void getSubscribeRequest(subscribeRequest *mySubscribeRequest, 
	message myMessage);

// Get capabilitiy notification from the message buffer
void getCapabilityNotifification(
	capabilitiesNotification *myCapabilityNotification, message myMessage);
	
// Get interest notification from the message buffer
void getInterestNotification(interestsNotification *myInterestNotification,
	message myMessage);
	
// Get subscribe instruction from the message buffer
void getSubscribeInstruction(subscribeInstruction *mySucscribeInstruction,
	message myMessage);
	
// Get safety alarm form the message buffer
void getSafetyAlarm(safetyAlarm *mySafetyAlarm, message myMessage);

// Get raw data from the message buffer
void getRawData(rawData *myRawData, message myMessage);

// Scans the address book for nodes that are interested in data contained in the
// passed in notification and sends data to these nodes.  Returns the number of
// nodes notified.
uint8 notifyInterestedNodes(notification myNotification);

