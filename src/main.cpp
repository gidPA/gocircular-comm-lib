#include "MessageExchange.h"
#include "Arduino.h"

MessageExchange messageExchange;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  messageExchange.setUartDevice(&Serial2);
  messageExchange.setUartMonitoringDevice(&Serial);

  messageExchange.createNewMessage(ITEM_ENTRY);
  messageExchange.setItemEntryStatus(ACCEPTED);
  messageExchange.setItemSize(LARGE);
  messageExchange.setItemType(PLASTIC_COLOURED);
  messageExchange.setItemPoint(40);

}


void loop()
{
  if(Serial2.available()){
    Serial.println("Incoming message");
    messageExchange.handleIncomingMessage();
    messageExchange.previewMessage();
  }

  // messageExchange.previewMessage();
  // messageExchange.sendMessage();
  // Serial1.println("test");
  // delay(3000);
}
