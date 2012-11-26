

#include "coordinator.h"

// Handles interests notification, replying to the notifying node with address
// entrees for all nodes that measure its interests
void handleInterestsNotification(message *sourceMessage,
    interestsNotification *myInterestsNotification)
{
    uint16 nodesToNotify[MY_ADDRESS_BOOK_LENGTH];
    uint8 numNodesToNotify, i = 0;
    address_entry *capableNodeEntry;
    
    // Add (or update) entry for notifying node
    if (isInAddressBook(sourceMessage->sourceNetworkAddress) > 0)
    {
        // Node exists, update capabilities
        capableNodeEntry = getAddressEntryPointer(
            sourceMessage->sourceNetworkAddress);
        
        capableNodeEntry->numInterests = myInterestsNotification->numInterests;
        
        memcpy(capableNodeEntry->interests, myInterestsNotification->interests, 
            capableNodeEntry->numInterests);
        
    }
    else 
    {
        // Node does not exist in address book, create it
        addNodeToAddressBook(sourceMessage->networkAddress, 
            sourceMessage->macHi, sourceMessage->macLow, sourceMessage->locale, 
            myInterestsNotification->numInterests,
            myInterestsNotification->interests,
            0, getMyCapabilitiesPtr());
        
    }
    
    // Determine how many nodes need to be notified
    numNodesToNotify = numNodesInLocalesWithInterests(
        myInterestsNotification->numInterests, myInterestsNotification->locales,
        myInterestsNotification->interests);
    
    // Retreive list of nodes
    nodesInLocalesWithInterests(nodesToNotify, 
        myInterestsNotification->numInterests, myInterestsNotification->locales,
        myInterestsNotification->interests);
    
    // Instruct notifying node to subscribe to nodes with capabilities
    for (i = 0; i < numNodesToNotify; i++)
    {
        // Point to capable node address entry
        capableNodeEntry = getAddressEntryPointer(nodesToNotify[i]);
        
        sendSubscribeInstruction(sourceMessage->sourceNetworkAddress, 
            capableNodeEntry->networkAddress, capableNodeEntry->macHi,
            capableNodeEntry->macLow, capableNodeEntry->locale,
            capableNodeEntry->numCapabilities, capableNodeEntry->capabilities);
        
    }
    
}


// Handles capability notification, sending all interested nodes a subscribe
// instruction for the notifying node
void handleCapabilitiesNotification(message *sourceMessage,
    capabilitiesNotification *myCapabilitiesNotification);
{
    uint16 nodesToNotify[MY_ADDRESS_BOOK_LENGTH];
    uint8 numNodesToNotify, i = 0;
    address_entry *capableNodeEntry;
    
    // Add (or update) entry for notifying node
    if (isInAddressBook(sourceMessage->sourceNetworkAddress) > 0)
    {
        // Node exists, update capabilities
        capableNodeEntry = getAddressEntryPointer(
            sourceMessage->sourceNetworkAddress);
        
        capableNodeEntry->numCapabilites = myCapabilitiesNotification->numCapabilities;
        
        memcpy(capableNodeEntry->capabilities, 
            myCapabilitiesNotification->capabilities, 
            capableNodeEntry->numCapabilites);
        
    }
    else 
    {
        // Node does not exist in address book, create it
        addNodeToAddressBook(sourceMessage->networkAddress, 
            sourceMessage->macHi, sourceMessage->macLow, sourceMessage->locale, 
            0, getMyInterestsPtr(),
            myCapabilitiesNotification->numCapabilities, 
            myCapabilitiesNotification->capabilities);
        
    }
    
    // Point to capable node address entry
    capableNodeEntry = getAddressEntryPointer(sourceMessage->sourceNetworkAddress);
    
    // Determine how many nodes need to be notified
    numNodesToNotify = numNodesWithInterestsInLocale(
        myCapabilitiesNotification->numCapabilities, 
        sourceMessage->sourceLocale,
        myCapabilitiesNotification->capabilities);
    
    // Retreive list of nodes to send subscribe instructions to
    nodesWithInterestsInLocale(nodesToNotify, 
        myCapabilitiesNotification->numCapabilities, 
        sourceMessage->sourceLocale,
        myCapabilitiesNotification->capabilities);
    
    // Instruct each node in the list to subscribe to the notifying node
    for (i = 0; i < numNodesToNotify; i++)
    { 
        sendSubscribeInstruction(nodesToNotify[i], 
            capableNodeEntry->networkAddress, capableNodeEntry->macHi,
            capableNodeEntry->macLow, capableNodeEntry->locale,
            capableNodeEntry->numCapabilities, capableNodeEntry->capabilities);
        
    }
    
}   
    