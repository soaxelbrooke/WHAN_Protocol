// WHAN Protocol header file
// By Stuart Owen
//
// Change Log:
//  11/8/2012   - Created

#include <my.h>
#include <message.h>
#include <address_book.h>

// Send a message via the Zigbee Transmit Request command
// See pg 89 of ftp://ftp1.digi.com/support/documentation/90000976_C.pdf
void sendMessage(uint32 macHi, uint32 macLow, uint16 networkAddress, 
    uint8 rfDataLength, uint8 *rfData);

// Get message from the receive buffer and put it into the passed in variables
void getMessage(uint32 *macHi, uint32 *macLow, uint16 *networkAddress,
    uint8 *rfData)

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

#IFDEF COORDINATOR
// Subscribe Instruction
void sendSubscribeInstruction(uint16 networkAddress, uint16 networkAddress,
    uint32 macHi, uint32 macLow, uint8 locale, uint8 numberOfCapabilities,
    uint8 *capabilities);

#ENDIF

// Response
// TODO: better define this!
void sendResponse(uint16 networkAddress, data?);

// Capability/Interest Notifications
void sendCapabilitiesNotification(uint8 capabilitiesLength, 
    uint8 *capabilities);
void sendInterestsNotification(uint8 interestsLength, uint8 *interests);

// Safety Alarms
void sendSafetyAlarm(uint8 riskType, uint8 riskLocale);

// Raw Data
void sendRawData(uint8 dataLength, uint8 *data);

// Scans the address book for nodes that are interested in data contained in the
// passed in notification and sends data to these nodes.  Returns the number of
// nodes notified.
uint8 notifyInterestedNodes(notification myNotification);

