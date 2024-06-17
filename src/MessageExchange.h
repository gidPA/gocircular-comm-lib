#ifndef MESSAGE_EXCHANGE_H
#define MESSAGE_EXCHANGE_H

#include <Arduino.h>

#define MESSAGE_SIZE 10
#define MESSAGE_START_CODE 252
#define MESSAGE_END_CODE 254

enum messageTopic
{
    TOPIC_ERR,
    BEGIN_TRANSACTION,     // begin transaction, no payloads
    ITEM_ENTRY,         //an item is entered, 4 payloads (status, material, size, points granted)
    SET_MEMBER_MODE,       // specify whether the following transaction is going to run with a companion app, no payload.
    SET_EXCHANGE_RATE,     // set exchange rate (to be used after server triggers system-wide exchange rate changes). 2 payloads.
    READY_FOR_TRANSACTION, // tell IoT gateway that system is ready for transaction. In turn, IoT gateway forwards this to the mobile app. No payload.
    TRANSACTION_COMPLETE,  // finish transaction. No payloads.
    BIN_FULL,              // trigger an alert to be sent the server that this particular bin is full. 1 payload (specifies the full bin)
    COIN_DISPENSER_ALERT   // trigger an alert to be sent to the server that the coin dispenser has ran out of coins.
};

enum itemType // specify the material type of an item
{
    ITEM_ERR,
    PLASTIC_COLOURED,    // for colored plastic bottle
    PLASTIC_TRANSPARENT, // for transparent plastic bottle
    METAL                // for metallic cans
};

enum itemSize
{
    SIZE_ERR,
    SMALL,  // size = x < y < x
    MEDIUM, // size = x < y < x
    LARGE   // size = x < y < x
};

enum itemStatus
{
    STAT_ERR,
    DECLINED,
    ACCEPTED,
    PENDING
};

class MessageExchange
{
private:
    byte message[MESSAGE_SIZE];
    HardwareSerial *uartDevice;
    HardwareSerial *uartMonitoringDevice;

    void warnIfPayloadDoesNotMatchTopic(messageTopic type, const char *typeName);


    void clearSerialBuffer();

    const char* getMessageTopicName(messageTopic);
    const char* getItemTypeName(itemType);
    const char* getItemSizeName(itemSize);
    const char* getItemStatusName(itemStatus);

public:
    MessageExchange();

    void setUartDevice(HardwareSerial *device);
    void setUartMonitoringDevice(HardwareSerial *device);
    void createNewMessage(messageTopic type);


    /**
     * Methods for ITEM_ENTRY related messages
    */

    void setItemEntryStatus(itemStatus status);
    byte getItemEntryStatus();

    void setItemType(itemType type);
    byte getItemType();

    void setItemSize(itemSize size);
    byte getItemSize();

    void setItemPoint(byte point);
    byte getItemPoint();

    
    /**
     * Methods for SET_EXCHANGE_RATE related messages
    */

    //void setExchangeRate(byte )

    /**
     * Methods for Exchanging Messages
    */

    void previewMessage();
    
    //Send message after message has been set
    void sendMessage();
    byte handleIncomingMessage();
};

#endif // MESSAGE_EXCHANGE_H
