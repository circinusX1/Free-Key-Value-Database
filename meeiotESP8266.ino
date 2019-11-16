/*

    Based on Sample from:  Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

int kk;
const char* ssid = "marius";
const char* password = "z.......";
const char* host = "www.meeiot.org";
const int httpsPort = 443;

const char* fingerprint = "48:3B:E1:2E:C7:9F:DC:FC:5E:A7:31:D7:FC:A8:2A:B2:72:D4:D5:0A";

WiFiClientSecure client;

void meePut(const char* value);
void meeGet(void);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
}

void loop() {

  char value[32];
  sprintf(value,"%05d",kk++);
  meePut(value);
  delay(10000);
  meeGet();
  delay(10000);
}

void meePut(const char* value)
{
  Serial.print("PUT connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/esp8266-1=";
  url+=value;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("PUT GOT:");
  Serial.println(line);
  Serial.println("conclosed");
  
}

void meeGet(void)
{
  Serial.print("GET connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
    
  // getting a value from meeiot
  String url = "/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/get/esp8266-1";
  
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("GET GOT:");
  Serial.println(line);
  Serial.println("conclosed");

}

