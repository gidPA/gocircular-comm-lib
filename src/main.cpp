#include "MessageExchange.h"

#define SERIAL_BUFFER_SIZE 128

MessageExchange messageExchange;

// void handleNewMessage() {
//   byte topic = messageExchange.handleIncomingMessage();
  
//   Serial.println("Executed handleIncomingMessage()");
//   Serial.print("Topic: ");
//   Serial.println(int(topic));
  
//   switch(topic) {
//     case 2:
//       Serial.println("Handling ITEM_ENTRY");
//       // Handle item masuk di sini
//       messageExchange.previewMessage();
//       break;
//     case ITEM_ERR:
//       Serial.println("Handling ITEM_ERR");
//       break;
//     default:
//       Serial.println("Unhandled Topic");
//       break;
//   }
// }
void handleMessage();


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  messageExchange.setUartDevice(&Serial2);
  messageExchange.setUartMonitoringDevice(&Serial);
}

void loop() {
  if (messageExchange.uartDevice->available() > 0) {
    messageExchange.uartMonitoringDevice->println(" ");
    messageExchange.uartMonitoringDevice->println("Incoming message");


    //messageExchange.handleIncomingMessage();

    handleMessage();
  }
}


void handleMessage(){
  byte messageTopic = messageExchange.handleIncomingMessage();

  messageExchange.previewMessage();

  switch(messageTopic){
    case ITEM_ENTRY:
      Serial.println("ITEM_ENTRY MESSAGE RECEIVED");
      break;

    default:
      Serial.println("Unhandled Topic");
      break;
  }
}
