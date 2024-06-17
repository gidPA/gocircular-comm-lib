#include "MessageExchange.h"
#include "Arduino.h"

#define SERIAL_BUFFER_SIZE 128

MessageExchange messageExchange;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  messageExchange.setUartDevice(&Serial2);
  messageExchange.setUartMonitoringDevice(&Serial);

  // messageExchange.createNewMessage(ITEM_ENTRY);
  // messageExchange.setItemEntryStatus(ACCEPTED);
  // messageExchange.setItemSize(LARGE);
  // messageExchange.setItemType(PLASTIC_COLOURED);
  // messageExchange.setItemPoint(40);
}

void loop() {
  if (messageExchange.uartDevice->available() > 0) {
    messageExchange.uartMonitoringDevice->println(" ");
    messageExchange.uartMonitoringDevice->println("Incoming message");

    //Handle semua topik message di sini
  }


}

