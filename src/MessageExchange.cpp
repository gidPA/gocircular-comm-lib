#include "MessageExchange.h"

MessageExchange::MessageExchange() : uartDevice(nullptr), uartMonitoringDevice(nullptr) {}

void MessageExchange::setUartDevice(HardwareSerial *device)
{
    uartDevice = device;
}

void MessageExchange::setUartMonitoringDevice(HardwareSerial *device)
{
    uartMonitoringDevice = device;
}

void MessageExchange::createNewMessage(messageTopic topic)
{
    // memset(message, 0, sizeof(message)); // initialize message array to 0
    message[0] = MESSAGE_START_CODE;
    message[1] = (byte)topic;
    message[MESSAGE_SIZE - 1] = MESSAGE_END_CODE;
}

void MessageExchange::setItemEntryStatus(itemStatus status)
{
    warnIfPayloadDoesNotMatchTopic(ITEM_ENTRY, "ITEM_ENTRY");
    message[2] = (byte)status;
}

byte MessageExchange::getItemEntryStatus()
{
    return message[2];
}

void MessageExchange::setItemType(itemType type)
{
    warnIfPayloadDoesNotMatchTopic(ITEM_ENTRY, "ITEM_ENTRY");
    message[3] = (byte)type;
}

byte MessageExchange::getItemType()
{
    return message[3];
}

void MessageExchange::setItemSize(itemSize size)
{
    warnIfPayloadDoesNotMatchTopic(ITEM_ENTRY, "ITEM_ENTRY");
    message[4] = (byte)size;
}

byte MessageExchange::getItemSize()
{
    return message[4];
}

void MessageExchange::setItemPoint(byte point)
{
    warnIfPayloadDoesNotMatchTopic(ITEM_ENTRY, "ITEM_ENTRY");
    message[5] = point;
}

byte MessageExchange::getItemPoint()
{
    return message[5];
}

void MessageExchange::previewMessage()
{
    int previewLength = sizeof(message) / sizeof(message[0]);
    char buffer[25];
    strcpy(buffer, "");

    for (int i = 0; i < previewLength; i++)
    {
        char temp[10];
        itoa(message[i], temp, 10);
        strcat(buffer, temp);
        if (i < previewLength - 1)
        {
            strcat(buffer, " ");
        }
    }

    if (uartMonitoringDevice != nullptr)
    {
        uartMonitoringDevice->print("[MessageExchange Preview]");
        uartMonitoringDevice->println(buffer);

        uartMonitoringDevice->println(getMessageTopicName((messageTopic)message[1]));
        uartMonitoringDevice->println(getItemTypeName((itemType)message[3]));
        uartMonitoringDevice->println(getItemSizeName((itemSize)message[4]));
        uartMonitoringDevice->println(getItemStatusName((itemStatus)message[2]));
    }
    else
    {
        Serial.println("UART monitoring device not set."); // Handle the error case where uartMonitoringDevice is not set
    }
}

void MessageExchange::sendMessage()
{
    if (uartDevice != nullptr)
    {
        uartDevice->write((byte *)message, (MESSAGE_SIZE / sizeof(message[0])));
        // uartDevice->println("Incoming Message");
    }
    else
    {
        if (uartMonitoringDevice != nullptr)
        {
            uartMonitoringDevice->println("UART device not set."); // Handle the error case where uartDevice is not set
        }
        else
        {
            Serial.println("UART device not set."); // Handle the error case where uartDevice and uartMonitoringDevice are not set
        }
    }
}

byte MessageExchange::handleIncomingMessage()
{
    uint8_t byteData;
    int prev = 0;
    int counter = 0;

    byte header = uartDevice->peek();

    if (header == MESSAGE_START_CODE)
    {
        while (true)
        {
            byteData = uartDevice->read();
            if (byteData != 255)
            {
                message[counter] = byteData;
                counter += 1;
            }
            if (byteData == 254)
            {
                prev = 1;
                break;
            }
        }
    }
    else
    {
        clearSerialBuffer();
    }

    if (prev > 0)
    {
        for (int i = 0; i < 10; i++)
        {
            uartMonitoringDevice->print(message[i]);
            uartMonitoringDevice->print(" ");
            if (message[i] == 254)
            {
                uartMonitoringDevice->println();
                break;
            }
        }
    }

    return message[1];
}

void MessageExchange::warnIfPayloadDoesNotMatchTopic(messageTopic topic, const char *topicName)
{
    //    if (message[0] != topic)
    //    {
    //        char buffer[100];
    //        snprintf(buffer, 100, "[WARNING!] One or more payload does not match the message topic: %s. Message parsing is likely going to misbehave.", topicName);
    //        if (uartMonitoringDevice != nullptr)
    //        {
    //            uartMonitoringDevice->println(buffer);
    //        }
    //        else
    //        {
    //            Serial.println(buffer); // Handle the error case where uartMonitoringDevice is not set
    //        }
    //    }
}

void MessageExchange::clearSerialBuffer()
{
    while (uartDevice->available() > 0)
    {
        uartDevice->read();
    }
}

// Function to convert messageTopic enum to string
const char *MessageExchange::getMessageTopicName(messageTopic topic)
{
    switch (topic)
    {
    case TOPIC_ERR:
        return "TOPIC_ERR";
    case BEGIN_TRANSACTION:
        return "BEGIN_TRANSACTION";
    case ITEM_ENTRY:
        return "ITEM_ENTRY";
    case SET_MEMBER_MODE:
        return "SET_MEMBER_MODE";
    case SET_EXCHANGE_RATE:
        return "SET_EXCHANGE_RATE";
    case READY_FOR_TRANSACTION:
        return "READY_FOR_TRANSACTION";
    case TRANSACTION_COMPLETE:
        return "TRANSACTION_COMPLETE";
    case BIN_FULL:
        return "BIN_FULL";
    case COIN_DISPENSER_ALERT:
        return "COIN_DISPENSER_ALERT";
    default:
        return "UNKNOWN_TOPIC";
    }
}

// Function to convert itemType enum to string
const char *MessageExchange::getItemTypeName(itemType type)
{
    switch (type)
    {
    case ITEM_ERR:
        return "ITEM_ERR";
    case PLASTIC_COLOURED:
        return "PLASTIC_COLOURED";
    case PLASTIC_TRANSPARENT:
        return "PLASTIC_TRANSPARENT";
    case METAL:
        return "METAL";
    default:
        return "UNKNOWN_ITEM_TYPE";
    }
}

// Function to convert itemSize enum to string
const char *MessageExchange::getItemSizeName(itemSize size)
{
    switch (size)
    {
    case SIZE_ERR:
        return "SIZE_ERR";
    case SMALL:
        return "SMALL";
    case MEDIUM:
        return "MEDIUM";
    case LARGE:
        return "LARGE";
    default:
        return "UNKNOWN_SIZE";
    }
}

// Function to convert itemStatus enum to string
const char *MessageExchange::getItemStatusName(itemStatus status)
{
    switch (status)
    {
    case STAT_ERR:
        return "STAT_ERR";
    case DECLINED:
        return "DECLINED";
    case ACCEPTED:
        return "ACCEPTED";
    case PENDING:
        return "PENDING";
    default:
        return "UNKNOWN_STATUS";
    }
}
