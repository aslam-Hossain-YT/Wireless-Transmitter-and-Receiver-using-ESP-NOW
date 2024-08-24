#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t receiverMacAddress[] = {0xC4, 0x5B, 0xBE, 0x96, 0x9C, 0xF8};  //C4:5B:BE:96:9C:F8

struct PacketData
{
  byte switch1Value;
  byte switch2Value;
  byte switch3Value;
  byte switch4Value;    
};
PacketData data;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
}

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("Succes: Initialized ESP-NOW");
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("Succes: Added peer");
  } 
  pinMode(15,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
  pinMode(23,INPUT_PULLUP); 
}
 
void loop() 
{
  data.switch1Value   = !digitalRead(15);
  data.switch2Value   = !digitalRead(5);
  data.switch3Value   = !digitalRead(21);
  data.switch4Value   = !digitalRead(23);  
  
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &data, sizeof(data));
  if (result == ESP_OK) 
  {
    Serial.println("Sent with success");
  }
  else 
  {
    Serial.println("Error sending the data");
  }    
  delay(30);
}