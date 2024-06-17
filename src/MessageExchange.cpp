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
    //memset(message, 0, sizeof(message)); // initialize message array to 0
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
        //uartDevice->println("Incoming Message");
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
    if (uartDevice != nullptr)
    {
        int nextByte = Serial.peek();

        if(nextByte == MESSAGE_START_CODE){
            //uartDevice->readBytesUntil(MESSAGE_END_CODE, message, MESSAGE_SIZE - 1 );
            for (int i = 0; i < MESSAGE_SIZE - 1; i++){
                message[i] = (byte)uartDevice->read();
            }
            
        }
        clearSerialBuffer();
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

void MessageExchange::clearSerialBuffer() {
    while (uartDevice->available() > 0) {
        uartDevice->read();
    }
}
