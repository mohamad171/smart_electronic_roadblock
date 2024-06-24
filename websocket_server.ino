#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
const char* ssid     = "Redmi";
const char* password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiClient client;
#define LED 2
const int d0 = D0;
const int d1 = D1;

void initWIFI(){
    
    
    Serial.println("Connecting to ");
    Serial.println(ssid); 
 
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) 
      {
        delay(500);
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
      }
    Serial.println("");
    Serial.println("WiFi connected");
    digitalWrite(LED, LOW);
    Serial.println("Board IP:");
    Serial.println(WiFi.localIP());
    initWebSocket();
}


void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.begin();
  Serial.printf("Socket connection run...");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data,"open") == 0) {
      digitalWrite(d0,LOW);
      Serial.println("Now should open");
    }
    if (strcmp((char*)data,"close") == 0) {
      digitalWrite(d0,HIGH);
      Serial.println("Now should close");
    }
  }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      Serial.printf("New Message");
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      Serial.printf("Error");
      break;
  }
}



void setup() {
    pinMode(LED, OUTPUT);
    pinMode(d0, OUTPUT);
    Serial.begin(115200);

}
void loop(){

  if(WiFi.status() != WL_CONNECTED){
    initWIFI();
  }
  
}
