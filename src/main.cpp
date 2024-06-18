#include "MessageExchange.h"

#define SERIAL_BUFFER_SIZE 128

MessageExchange messageExchange;

void handleMessage();

byte itemEntryCounter = 0;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  messageExchange.setUartDevice(&Serial2);
  messageExchange.setUartMonitoringDevice(&Serial);
}

void loop()
{
  if (messageExchange.uartDevice->available() > 0)
  {
    messageExchange.uartMonitoringDevice->println(" ");
    messageExchange.uartMonitoringDevice->println("Incoming message");

    // messageExchange.handleIncomingMessage();

    handleMessage();
  }
}

void handleMessage()
{
  byte messageTopic = messageExchange.handleIncomingMessage();

  messageExchange.previewMessage();

  switch (messageTopic)
  {
  case ITEM_ENTRY:
    Serial.println("ITEM_ENTRY MESSAGE RECEIVED");
    byte entryStatus, itemSize, itemType;
    uint16_t poin;
    entryStatus = messageExchange.getItemEntryStatus();
    itemSize = messageExchange.getItemSize();
    itemType = messageExchange.getItemType();

    if (entryStatus == ACCEPTED)
    {
      Serial.println("===============");
      Serial.println("BARANG DITERIMA");

      if (itemSize == MEDIUM && itemType == PLASTIC_TRANSPARENT)
      {
        // handle poin di sini
        // kemudian akumulasikan bareng poin yang terkumpul sebelumnya
        Serial.println("Poin = 50");
      }
    }

    else if (entryStatus == DECLINED)
    {
    }
    else if (entryStatus == PENDING)
    { // PENDING
    }
    break;

  default:
    Serial.println("Unhandled Topic");
    break;
  }
}
