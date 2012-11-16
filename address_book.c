

#include "address_book.h"

static address_entry addressBook[MY_ADDRESS_BOOK_LENGTH];

// TODO: make some mechanism for storing this in non-volatile memory!

// Add a device to the address book (returns non-zero if address book is full)
// Only interests that match this nodes capabilities are stored
uint8 addNodeToAddressBook(uint16 networkAddress, uint32 macHi, uint32 macLow, 
    uint8 locale, uint8 numInterests, uint8 *interests, uint8 numCapabilities,
    uint8 *capabilities)
{
    uint8 result = 0;
    uint8 i = 0;
    
    // Check if address book is full
    if (numFreeAddressEntries() != 0)
    {
        // If address book isn't full, find first free entry
        for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
        {
            if (addressBook[i].locale == 0) // If locale is zero, entry is free
            {
                addressBook[i].networkAddress = networkAddress;
                addressBook[i].macHi = macHi;
                addressBook[i].macLow = macLow;
                addressBook[i].locale = locale;
                addressBook[i].numInterests = numInterests;
                memcpy(addressBook[i].interests, interests, numInterests);
                addressBook[i].numCapabilities = numCapabilities;
                memcpy(addressBook[i].capabilities, capabilities, 
                    numCapabilities);
                
                break;
                
            }
            
        }
        
    } else {
        result = 1;
    }
    
    return result;
    
}

// Remove a node form the address book, returns non zero if node not found.
uint8 removeNodeFromAddressBook(uint16 networkAddress)
{
    uint8 i = 0;
    uint8 result = 1;
    
    // If address book isn't full, find first free entry
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress) 
        {
            addressBook[i].locale = 0;
                
            result = 0;
            
            break;
            
        }
        
    }
    
    return result;
    
}

// Returns the number of address book entries
uint8 numAddressEntries(void)
{
    return (MY_ADDRESS_BOOK_LENGTH - numFreeAddressEntries());
}

// Returns number of free spaces in address book
uint8 numFreeAddressEntries(void)
{
    uint8 result = 0;
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale == 0)
        {
            result += 1;
            
        }
        
    }
    
    return result;
    
}

// Returns nonzero if network address is not in address book
uint8 isInAddressBook(uint16 networkAddress)
{
    uint8 result = 1;
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress)
        {
            result = 0;
            
            break;
            
        }
        
    }
    
    return result;
    
}

// Gets the low 32 bits of the MAC associated with the passed in network address
uint32 getMacLow(uint16 networkAddress)
{
    uint32 result = 0;
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress)
        {
            result = addressBook[i].macLow;
            
            break;
            
        }
        
    }
    
    return result;
    
}

// Gets the hi 32 bits of the MAC associated with the passed in network address
uint32 getMacHigh(uint16 networkAddress)
{
    uint32 result = 0;
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress)
        {
            result = addressBook[i].macHi;
            
            break;
            
        }
        
    }
    
    return result;
    
}

// Returns a count of the number of nodes in the address book with the passed in
// interest
uint8 numNodesWithInterest(uint8 interest)
{
    uint8 result = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < addressBook[i].numInterests; j++)
        {
            if (addressBook[i].interests[j] == interest)
            {
                result += 1;
                break;
                
            }
            
        }
        
    }
    
    return result;
    
}

// Returns a count of the number of nodes in the address book with any of the 
// passed in interest
uint8 numNodesWithInterests(uint8 numInterests, uint8 *interests)
{
    uint8 result = 0;
    uint8 i = 0, j = 0, k = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numInterests; j++)
        {
            for (k = 0; k < addressBook[i].numInterests; k++)
            {
                if (addressBook[i].interests[k] == interests[j])
                {
                    // If a match is found, increment count and break k-for-loop
                    result += 1;
                    
                    break;
                    
                }
                
            }
            
        }
        
    }
    
    return result;
    
}

// Returns a count of the number of nodes in the address book in the passed in
// locale
uint8 numNodesInLocale(uint8 locale)
{
    uint8 result = 0;
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale == locale)
        {
            result += 1;
            
        }
        
    }
    
    return result;
    
}

// Returns a count of the number of nodes in the address book in the passed in 
// locales
uint8 numNodesInLocales(uint8 numLocales, uint8 *locales)
{
    uint8 result = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numLocales; j++)
        {
            if (addressBook[i].locale == locales[j])
            {
                result += 1;
                
                break;
                
            }
            
        }
        
    }
    
    return result;
    
}

// Returns a count of nodes with given capabilities
uint8 numNodesWithCapability(uint8 capability)
{
    uint8 result = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < addressBook[i].numCapabilities; j++)
        {
            if (addressBook[i].capabilities[j] == capability)
            {
                result += 1;
                break;
                
            }
            
        }
        
    }
    
    return result;
    
}

// Return a count of nodes with any of given capabilities
uint8 numNodesWithCapabilities(uint8 numCapabilities, uint8 *capabilities)
{
    uint8 result = 0;
    uint8 i = 0, j = 0, k = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numCapabilities; j++)
        {
            for (k = 0; k < addressBook[i].numCapabilities; k++)
            {
                if (addressBook[i].capabilities[k] == capabilities[j])
                {
                    // If a match is found, increment count and break k-for-loop
                    result += 1;
                    
                    break;
                    
                }
                
            }
            
        }
        
    }
    
}

// Returns a count of nodes in given locales with given interests
uint8 numNodesInLocalesWithInterests(uint8 numLocales, uint8 *locales, 
    uint8 numInterests, uint8 *interests)
{
    return 0;
}

// Returns a count of node sin given locales with given capabilities
uint8 numNodesInLocalesWithCapabilities(uint8 numLocales, uint8 *locales, 
    uint8 numCapabilities, uint8 *capabilities)
{
    return 0;
}
    
// Populates a passed in array with list of nodes in given locale.
// Assumes that nodes have been counted and that proper memory has been 
// allocated.
void nodesInLocale(uint16 *networkAddresses, uint8 locale)
{
    uint8 i = 0;
    uint8 nodesFound = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale == locale)
        {
            networkAddresses[nodesFound] = addressBook[i].networkAddress;
            
            nodesFound += 1;
            
        }
        
    }
    
}

void nodesInLocales(uint16 *networkAddresses, uint8 numLocales, uint8 *locales)
{
    uint8 nodesFound = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numLocales; j++)
        {
            if (addressBook[i].locale == locales[j])
            {
                networkAddresses[nodesFound] = addressBook[i].networkAddress;
                
                nodesFound += 1;
                
            }
            
        }
        
    }
}

// Populates a passed in array with list of nodes with given interests.
// Assumes that nodes have been counted and that proper memory has been 
// allocated.
void nodesWithInterest(uint16 *networkAddresses, uint8 interest)
{
    uint8 nodesFound = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < addressBook[i].numInterests; j++)
        {
            if (addressBook[i].interests[j] == interest)
            {
                networkAddresses[nodesFound] = addressBook[i].networkAddress;
                
                nodesFound += 1;
                
            }
            
        }
        
    }
    
}

void nodesWithInterests(uint16 *networkAddresses, uint8 numInterests, 
    uint8 *interests)
{
    uint8 nodesFound = 0;
    uint8 i = 0, j = 0, k = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numInterests; j++)
        {
            for (k = 0; k < addressBook[i].numInterests; k++)
            {
                if (addressBook[i].interests[k] == interests[j])
                {
                    networkAddresses[nodesFound] = addressBook[i].networkAddress;
                    
                    nodesFound += 1;
                    
                }
                
            }
            
        }
        
    }
    
}

// Populates a passed in network address array with list of nodes with given 
// capability.  Assumes that nodes have been counted and that proper memory has 
// been allocated.
void nodesWithCapability(uint16 *networkAddresses, uint8 capability)
{
    uint8 nodesFound = 0;
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < addressBook[i].numCapabilities; j++)
        {
            if (addressBook[i].capabilities[j] == capability)
            {
                networkAddresses[nodesFound] = addressBook[i].networkAddress;
                
                nodesFound += 1;
                
            }
            
        }
        
    }
    
}

void nodesWithCapabilities(uint16 *networkAddresses, uint8 numCapabilities, 
    uint8 *capabilities)
{
    uint8 nodesFound = 0;
    uint8 i = 0, j = 0, k = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        for (j = 0; j < numCapabilities; j++)
        {
            for (k = 0; k < addressBook[i].numCapabilities; k++)
            {
                if (addressBook[i].capabilities[k] == capabilities[j])
                {
                    networkAddresses[nodesFound] = addressBook[i].networkAddress;
                    
                    nodesFound += 1;
                    
                }
                
            }
            
        }
        
    }
    
}

// Populates passed in network addres array with list of nodes in given locales
// with given interests or capabilities.  Assumes that nodes have been counted 
// and that proper memory has been allocated.
void nodesInLocalesWithInterests(uint16 *networkAddresses, uint8 numLocales,
    uint8 *locales, uint8 numInterests, uint8 *interests)
{
    
}

void nodesInLocalesWithCapabilities(uint16 *networkAddresses, uint8 numLocales,
    uint8 *locales, uint8 numCapabilities, uint8 *capabilities)
{
    
}

// Returns a list of all nodes in the network
void nodesInAddressBook(uint16 *networkAddresses)
{
    uint8 i = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].locale != 0)
        {
            networkAddresses[i] = addressBook[i].networkAddress;
            
        }
        
    }
    
}


// Returns the number of interests a node has
uint8 numNodeInterests(uint16 networkAddress)
{
    uint8 i = 0;
    uint8 result = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress)
        {
            result = addressBook[i].numInterests;
            
            break;
            
        }
        
    }
    
    return result;
    
}

// Writes a list of interests a node has to the passed in pointer
void nodeInterests(uint8 *interests, uint16 networkAddress)
{
    uint8 i = 0, j = 0;
    
    for (i = 0; i < MY_ADDRESS_BOOK_LENGTH; i++)
    {
        if (addressBook[i].networkAddress == networkAddress)
        {
            for (j = 0; j < addressBook[i].numInterests; j++)
            {
                interests[j] = addressBook[i].interests[j];
                
            }
            
            break;
            
        }
        
    }
    
}

