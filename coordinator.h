
#include "whan_protocol.h"

// Handles interest notification, replying to the notifying node with address
// entrees for all nodes that measure it's interests
void handleInterestsNotification(message *sourceMessage,
    interestsNotification *myInterestsNotification);


// Handles capability notification, sending all interested nodes a subscribe
// instruction for the notifying node
void handleCapabilitiesNotification(message *sourceMessage,
    capabilitiesNotification *myCapabilitiesNotification);
    
    