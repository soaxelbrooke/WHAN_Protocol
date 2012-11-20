/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <device.h>
#include "whan_protocol.h"
#include "light_timer.h"
#include "light_control.h"

// Define device operating modes
#define OPERATING_MODE_HOLD 0x00
#define OPERATING_MODE_AUTO 0x01
#define OPERATING_MODE_USER_CONTROL 0x02

void initComponents(void);

void serviceUserCommand(message myMessage);
void serviceNotification(message myMessage);
void serviceNotificationRequest(message myMessage);
void serviceSubscribeRequest(uint16 srcNetworkAddress, uint32 srcMacLow, 
    uint32 srcMacHi, message myMessage);
void serviceResponse(message myMessage);
void serviceInterestNotification(message myMessage);
void serviceCapabilityNotification(message myMessage);
void serviceSubscribeInstruction(message myMessage);
void serviceSubscribeInstruction(message myMessage);
void serviceSafetyAlarm(message myMessage);
void serviceRawData(message myMessage);
void actOnNotification(int32 notifData, uint8 notifType);


volatile uint8 messageReceived = 0x00;
static uint8 operatingMode = 0x00;
extern uint8 myInterests[];

// Current device state:
//	* Is it on or off?
//	* What operating mode is it in?
//	* What dimming level is it at? (on = 0x7fffffff, off = 0)
//	* Average watt usage since last power notification?
#define STATE_INDEX_POWERED 0
#define STATE_INDEX_OPERATING_MODE 1
#define STATE_INDEX_DIMMING_LEVEL 2
#define STATE_INDEX_AVERAGE_POWER 3
#define STATE_INDEX_ENVIRONMENT_LUMENS 4
#define NUM_STATE_VARIABLES 5
// Types for each device state variable
static uint8 stateType[] = {UNIT_TYPE_POWERED, UNIT_TYPE_OPERATING_MODE,
	UNIT_TYPE_DIMMING_LEVEL, UNIT_TYPE_WATTS, UNIT_TYPE_LUMENS};
// Data for each device state variable
static int32 stateCurrent[] = {0, 0, 0, 0, 0};
// Goals for each device state variable
static int32 stateGoal[] = {0, 0, 0, 0, 0};

// Define light delta threshold (what is "close enough"?)
#define LIGHT_DELTA_THRESHOLD 50 // Lumens



void main()
{
	
	uint8 messageData[MAX_SINGLE_MESSAGE_LENGTH];
	message myMessage;
	myMessage.data = messageData;
	uint32 srcMacHi = 0, srcMacLow = 0;
	uint16 srcNetworkAddress = 0;
	
/*	uint8 interests[] = {0x01, 0x02, 0x03};
	uint8 capabilities[] = {0x05};

	notification N;
	
	
	uint8 numNotifs = 2;
	uint8 notifTypes[] = {0x01, 0x03};
	int32 notifData[] = {0xa1a2a3a4, 0xb1b2b3b4};

	N.numNotifications = numNotifs;
	N.types = notifTypes;
	N.data = notifData;*/

    initComponents();
	
/*
	notifyInterestedNodes(N);
	
	addNodeToAddressBook(0x1234, 0x98765432, 0x12345678, 0xaa, 0x03, interests, 0x01, capabilities);

	notifyInterestedNodes(N);
	
	sendRawData(0x1234, 0x03, interests);*/
	
	
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        /* Place your application code here. */
		if (messageReceived != 0)
		{
			// Get message contents
			getMessageStruct(&srcNetworkAddress, &srcMacLow, &srcMacHi, 
				&myMessage);
			
			switch (myMessage.type) {
			case MESSAGE_TYPE_USER_COMMAND:
				serviceUserCommand(myMessage);
				break;
				
			case MESSAGE_TYPE_NOTIFICATION:
				serviceNotification(myMessage);
				break;
				
			case MESSAGE_TYPE_NOTIFICATION_REQUEST:
				serviceNotificationRequest(myMessage);
				break;
				
			case MESSAGE_TYPE_SUBSCRIBE_REQUEST:
				serviceSubscribeRequest(srcNetworkAddress, srcMacLow, srcMacHi,
					myMessage);
				break;
				
			case MESSAGE_TYPE_RESPONSE:
				serviceResponse(myMessage);
				break;
				
			case MESSAGE_TYPE_INTEREST_NOTIFICATION:
				serviceInterestNotification(myMessage);
				break;
				
			case MESSAGE_TYPE_CAPABILITY_NOTIFICATION:
				serviceCapabilityNotification(myMessage);
				break;
				
			case MESSAGE_TYPE_SUBSCRIBE_INSTRUCTION:
				serviceSubscribeInstruction(myMessage);
				break;
				
			case MESSAGE_TYPE_SAFETY_ALARM:
				serviceSafetyAlarm(myMessage);
				break;
				
			case MESSAGE_TYPE_RAW_DATA:
				serviceRawData(myMessage);
				break;
				
			default:
				// Command type not recognized
				break;
				
			}
			
			
			messageReceived = 0;
			
		}
		
    }
	
}

void initComponents(void)
{
	// Note!  Device state should be initialized before normal operation begins!
	
	Zigbee_UART_1_Start();
	initLight();
	RxInterrupt_Start();
	
}

// Should be called when a user command is received
void serviceUserCommand(message myMessage)
{
	uint8 i = 0;
	userCommand myUserCommand;
	
	// Populate user command from message
	getUserCommand(&myUserCommand, myMessage);
	
	if (myUserCommand.command == USER_COMMAND_CHANGE_VALUE)
	{
		// Change requested goal if possible
		if (myUserCommand.type == UNIT_TYPE_LOCALE)
		{
			setMyLocale((uint8)myUserCommand.data);
		}
		else 
		{
			for (i = 0; i < NUM_STATE_VARIABLES; i++)
			{
				if (stateType[i] == myUserCommand.type)
				{
					stateGoal[i] = myUserCommand.data;
					break;
					
				}
			
			}
			
		}
		
	}
	else if (myUserCommand.command == USER_COMMAND_PROMPT_ABILITIES)
	{
		// Send coordinator capabilities notification
		// TODO: should an interest notificaiton be sent as well?
		// sendMyInterestsNotification();
		// CyDelay(100);
		sendMyCapabilitiesNotification();
		
	}
	
}

// Should be called when a notification is received
void serviceNotification(message myMessage)
{
	#ifdef ACTOR
		// Create notificaiton in memory
		notification myNotification;
		int32 notifData[MY_NUM_INTERESTS];
		uint8 notifTypes[MY_NUM_INTERESTS];
		uint8 *interests = getMyInterestsPtr();
		uint8 i = 0, k = 0;
		
		// Map notification struct contents
		myNotification.numNotifications = MY_NUM_INTERESTS;
		myNotification.data = notifData;
		myNotification.types = notifTypes;
		
		// Retreive notification data from message
		getNotification(&myNotification, myMessage);
		
		for (i = 0; i < myNotification.numNotifications; i++)
		{
			for (k = 0; k < MY_NUM_INTERESTS; k++)
			{
				if (myNotification.types[i] == interests[k])
				{
					actOnNotification(myNotification.data[i], 
						myNotification.types[i]);
					
				}
				
			}
			
		}
		
	#endif
}

// Should be called when a notification request is received
void serviceNotificationRequest(message myMessage)
{
	#ifdef SENSOR
		// Create notification request struct
		notificationRequest myNotificationRequest;
		uint8 *capabilities = getMyCapabilitiesPtr();
		uint8 i = 0, k = 0;
		
		// Populate struct from message buffer
		getNotificationRequest(&myNotificationRequest, myMessage);
		
		
		for (i = 0; i < myNotificationRequest.numTypes; i++)
		{
			for (k = 0; k < MY_NUM_CAPABILITIES; k++)
			{
				if (myNotificationRequest.types[i] == capabilities[k])
				{
					
					// Insert application specific code here
					// Either give latest data or measure new data and send
					
				}
				
			}
			
		}
		
	#endif
}

// Should be called when a subscribe request is received
void serviceSubscribeRequest(uint16 srcNetworkAddress, uint32 srcMacLow, 
	uint32 srcMacHi, message myMessage)
{
	#ifdef SENSOR
		// Create subscribe request struct
		subscribeRequest mySubscribeRequest;
		
		// Populate struct from message
		getSubscribeRequest(&mySubscribeRequest, myMessage);
		
	    // Add node to address book (if there is space)
		// TODO: Check if the node is already in the address book (update entry)
	    if (numFreeAddressEntries() > 0)
	    {
			addNodeToAddressBook(srcNetworkAddress, srcMacHi, 
				srcMacLow, myMessage.sourceLocale, mySubscribeRequest.numTypes,
				mySubscribeRequest.types, 0, getMyCapabilitiesPtr());
				
		}
		
	#endif
}

// Should be called when a response is received
void serviceResponse(message myMessage)
{
	// Insert application specific code here
}

// Should be called when an interest notification is received
void serviceInterestNotification(message myMessage)
{
	#ifdef COORDINATOR
		
		// Insert application specific code here
	
	#endif
}

// Should be called when a capability notification is received
void serviceCapabilityNotification(message myMessage)
{
	#ifdef COORDINATOR
		
		// Insert application specific code here
		
	#endif
}

// Should be called when a Subscribe Instruction has been received
void serviceSubscribeInstruction(message myMessage)
{
	#ifdef ACTOR
	    // Get data needed to add node to address book
	    uint16 targetNetworkAddress = myMessage.data[1] + 
	        (myMessage.data[0] << 8);
	    uint32 targetMacHi = myMessage.data[5] + 
	        (myMessage.data[4] << 8) + 
	        (myMessage.data[3] << 16) + 
	        (myMessage.data[2] << 24);
	    uint32 targetMacLow = myMessage.data[9] + 
	        (myMessage.data[8] << 8) + 
	        (myMessage.data[7] << 16) + 
	        (myMessage.data[6] << 24);
	    uint8 targetLocale = myMessage.data[10];
	    uint8 targetNumCapabilities = myMessage.data[11];
	    uint8 *targetCapabilities = (uint8*)(myMessage.data + 12);
	    
	    // Add node to address book (if there is space)
	    if (numFreeAddressEntries() > 0)
	    {
	        addNodeToAddressBook(targetNetworkAddress, targetMacHi, 
				targetMacLow, targetLocale, 0, getMyInterestsPtr(), 
				targetNumCapabilities, targetCapabilities);
	    }
	    
	    // Request subscription from target node
	    sendSubscribeRequest(targetNetworkAddress, MY_NUM_INTERESTS, 
			getMyInterestsPtr());
    
	#endif
}

// Should be called when a safety alarm is received
void serviceSafetyAlarm(message myMessage)
{
	// Insert application specific code here
}

// Should be called when a raw data message is received
void serviceRawData(message myMessage)
{
	// Insert application specific code here
}

// Act on notification based on type, handles singular notifications
void actOnNotification(int32 notifData, uint8 notifType)
{
	if (notifType == UNIT_TYPE_LUMENS)
	{
		stateCurrent[STATE_INDEX_ENVIRONMENT_LUMENS] = notifData;
	
	}
	else if (notifType == UNIT_TYPE_OCCUPANTS)
	{
		if (operatingMode == OPERATING_MODE_AUTO)
		{
			turnOn();
			
			startTurnOffTimer();
			resetTurnOffTimer();
			
		}
		
	}
	
}

/* [] END OF FILE */
