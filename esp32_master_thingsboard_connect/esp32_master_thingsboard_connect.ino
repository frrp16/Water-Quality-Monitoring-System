#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ThingsBoard.h>


// WiFi and MQTT constants
#define WIFI_SSID "YOUR SSID"
#define WIFI_PASSWORD "YOUR PASSWORD"

#define MQTT_BROKER "broker.hivemq.com" // Broker Testing: https://www.hivemq.com/demos/websocket-client/
#define MQTT_PORT 8883
#define MQTT_TOPIC "FinalProject/B7/water_quality"

// ThingsBoard constants
#define THINGSBOARD_DEVICE_TOKEN "2zWTfsPhXE9syDTgYGOP"
#define THINGSBOARD_SERVER "thingsboard.cloud"

float temperature = 25;
float tds = 0;
float turbidity = 0;
long lastSend;

int status = WL_IDLE_STATUS;

// Certificates
const char* server_cert = R"(-----BEGIN CERTIFICATE-----
MIIEdTCCA12gAwIBAgIJAKcOSkw0grd/MA0GCSqGSIb3DQEBCwUAMGgxCzAJBgNV
BAYTAlVTMSUwIwYDVQQKExxTdGFyZmllbGQgVGVjaG5vbG9naWVzLCBJbmMuMTIw
MAYDVQQLEylTdGFyZmllbGQgQ2xhc3MgMiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0
eTAeFw0wOTA5MDIwMDAwMDBaFw0zNDA2MjgxNzM5MTZaMIGYMQswCQYDVQQGEwJV
UzEQMA4GA1UECBMHQXJpem9uYTETMBEGA1UEBxMKU2NvdHRzZGFsZTElMCMGA1UE
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjE7MDkGA1UEAxMyU3RhcmZp
ZWxkIFNlcnZpY2VzIFJvb3QgQ2VydGlmaWNhdGUgQXV0aG9yaXR5IC0gRzIwggEi
MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDVDDrEKvlO4vW+GZdfjohTsR8/
y8+fIBNtKTrID30892t2OGPZNmCom15cAICyL1l/9of5JUOG52kbUpqQ4XHj2C0N
Tm/2yEnZtvMaVq4rtnQU68/7JuMauh2WLmo7WJSJR1b/JaCTcFOD2oR0FMNnngRo
Ot+OQFodSk7PQ5E751bWAHDLUu57fa4657wx+UX2wmDPE1kCK4DMNEffud6QZW0C
zyyRpqbn3oUYSXxmTqM6bam17jQuug0DuDPfR+uxa40l2ZvOgdFFRjKWcIfeAg5J
Q4W2bHO7ZOphQazJ1FTfhy/HIrImzJ9ZVGif/L4qL8RVHHVAYBeFAlU5i38FAgMB
AAGjgfAwge0wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0O
BBYEFJxfAN+qAdcwKziIorhtSpzyEZGDMB8GA1UdIwQYMBaAFL9ft9HO3R+G9FtV
rNzXEMIOqYjnME8GCCsGAQUFBwEBBEMwQTAcBggrBgEFBQcwAYYQaHR0cDovL28u
c3MyLnVzLzAhBggrBgEFBQcwAoYVaHR0cDovL3guc3MyLnVzL3guY2VyMCYGA1Ud
HwQfMB0wG6AZoBeGFWh0dHA6Ly9zLnNzMi51cy9yLmNybDARBgNVHSAECjAIMAYG
BFUdIAAwDQYJKoZIhvcNAQELBQADggEBACMd44pXyn3pF3lM8R5V/cxTbj5HD9/G
VfKyBDbtgB9TxF00KGu+x1X8Z+rLP3+QsjPNG1gQggL4+C/1E2DUBc7xgQjB3ad1
l08YuW3e95ORCLp+QCztweq7dp4zBncdDQh/U90bZKuCJ/Fp1U1ervShw3WnWEQt
8jxwmKy6abaVd38PMV4s/KCHOkdp8Hlf9BRUpJVeEXgSYCfOn8J3/yNTd126/+pZ
59vPr5KW7ySaNRB6nJHGDn2Z9j8Z3/VyVOEVqQdZe4O/Ui5GjLIAZHYcSNPYeehu
VsyuLAOQ1xk4meTKCRlb/weWsKh/NEnfVqn3sF/tM+2MR7cwA130A4w=
-----END CERTIFICATE-----)";

const char* client_cert = R"(-----BEGIN CERTIFICATE-----
MIIDRTCCAi2gAwIBAgIUCWBU6rjEzz0BGwfAmmSPlRL8oN0wDQYJKoZIhvcNAQEL
BQAwMjELMAkGA1UEBhMCSUQxEzARBgNVBAgMCkphd2EgQmFyYXQxDjAMBgNVBAcM
BURlcG9rMB4XDTIzMTExMDAzNDYxOVoXDTI0MTEwOTAzNDYxOVowMjELMAkGA1UE
BhMCSUQxEzARBgNVBAgMCkphd2EgQmFyYXQxDjAMBgNVBAcMBURlcG9rMIIBIjAN
BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwmZZ/IVx7D25d6YCSHT+Q9mRSJlF
IZLsM15Bt60LomsiUuODWGiaqWKLKEV3MEB2YNJhRCSqfdeJC6uPNFBowr0+Q0g+
vrLrFw8zd5xa5FHf+xJVEKYGLkZD1SxOMNyON/9UzJAAkgQprIDHvJ+dl4JRPe/+
HeZrkbfnVJ5x/yFHChHQ4Jw3uQNczlcKEx5JqJYu49QRA5x90caPJAvpT2QTXj6S
nwXwld2syGtTHoQUh8QmIcqRUOxR7gvdIGy2kx6voo022B64gTeQZQLezK1ZzMPw
I1RJUiVdqHqShwj5Ui4bSiKZmZrW0wuqVX1mHtNuDJqIDGOKz3/gYNE0lQIDAQAB
o1MwUTAdBgNVHQ4EFgQUvZ3poWhN6ilJ1+oG4OkGDWwaY5swHwYDVR0jBBgwFoAU
vZ3poWhN6ilJ1+oG4OkGDWwaY5swDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0B
AQsFAAOCAQEAGQKB1hOVcJKOVTqc0dBi3UFgcE9jkzlHc1PaGsd6gNx+icsl8lDZ
gFK4pf4mgphPlHxPORKB9ShJjslu9etWd5cpG62JAeD4Y+5xgMMAC/prcFEnsNoM
mXU3jqE5xS5nwEiErLJOqbZ/QrjyJEQRh8blcJJ4hCCcYFZdxofmMRYvh0qG6Xem
A6qr8OUB++3RsjlTSGZ2oYE8EJCr087wyFqbSqlCQe+4vSZ1U54T7KF8kXCFhIVv
0ICcoYJB7wrK0nkyI2fQtG8Ro9RElDYnCNTsbgck3A3SZh4SK0K1IbJG81vxd5Sz
2yxX6UXv0u7zooaqfxn3d9iXdtN/eHL4rg==
-----END CERTIFICATE-----)";

const char* client_key = R"(-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDCZln8hXHsPbl3
pgJIdP5D2ZFImUUhkuwzXkG3rQuiayJS44NYaJqpYosoRXcwQHZg0mFEJKp914kL
q480UGjCvT5DSD6+susXDzN3nFrkUd/7ElUQpgYuRkPVLE4w3I43/1TMkACSBCms
gMe8n52XglE97/4d5muRt+dUnnH/IUcKEdDgnDe5A1zOVwoTHkmoli7j1BEDnH3R
xo8kC+lPZBNePpKfBfCV3azIa1MehBSHxCYhypFQ7FHuC90gbLaTHq+ijTbYHriB
N5BlAt7MrVnMw/AjVElSJV2oepKHCPlSLhtKIpmZmtbTC6pVfWYe024MmogMY4rP
f+Bg0TSVAgMBAAECggEAAMVvLfVJkmpcRgE+mRL9vz8puGQDRjLTBREmqZfiJ+7M
4A9PAhbIJkvhyXMdGe/niFLNV/vdQUqNnKI9mzOGdt9RU+sBdLg4/ops5diU87rv
bIuSEtLVy3Ez67CVTtRRLB+zMmyP1AR4Ndr8796iPWui7t/43h1/58AXBtrqEN0L
KHybMrHY+06KVEJ1Z2cqNH5S60mlvIxv3NcRqXw6pEoyjB0DzBGc2RuZfEXXGUe9
e0TKMqDRtGdtPumwB40bKBt3IUi58eLRU6CEJsrx09dufX0MbZE3bx5ww8fh68Jv
yR7gVT51BjltIG0Kna7BvFrNgAyev6WcyiVE75wLMQKBgQDx27W8gJuBJZ2FuPfJ
bGsaV6qUBHk0zl6g1YmAiNEV+s0woiHO64TgMq/KejnZCZaoAx0X+hgXlzPu/zo4
/4ahmL7uzK887lfJq+cRA+cXPy69lhyAKnbHcxpJc3mJBYqj4PfEc4gyBq4Oib04
Y+PX4dXENUYpvLDqXrP4vRn00wKBgQDNxEDSHAT4ZJteLDorIDtymzrz3swRLynE
GNwR/8SL7Vz0JuHGkpu7AEbmXpxvIeLT5OCwNHsCXgWIA4+6ih5+Hy1N4dLrhpCR
13A5KtwnRVzOgfBhlMjUP6i7X7CXpJZpkgmVvDml6xi+lfDdidkkxk6LdrgDdroS
5OxRHNfv9wKBgQDvNWIrHXwwILcK0mXpB11rAXgJy97hdpcrNeg4g5lgxtz5E/Ki
SFj0sc0iUv8yBYTHicTVuRrpAjhwUmkQhNJws7VsVG+hsQ6MvYPdVWHToT8mbfik
Xtt56wL90MNM3XlBGrmTyLzU1byDToIi6tEn7QpTdGl/0EuekqooA0hP1wKBgEDE
1raMVVAusu0otMKgabYAgMUEy21ZUCdGv/b2cFKvS4QavRS5T0XEp4yaGVmt6+1Q
7Rzp2BJ9r/YJUPOhyElg3OzX+YFjoRwXGV8VS6kc30KtihHdOH3+78jTLQ3+o83l
RxkX33mHkAllvySs4Gn/9fdWTkPg2l9aKby6fIovAoGBAMJDUCV/2TAhQ86lzO86
a8HxEXSj4BaDVNK/6GptnwUBAx/Otrp5LPYnElnXVjQ+aJtjpoZILj6pxoc31+14
1jhp0dAlDb4ijOo0NUKtCFY3OLCynhOlYjYf/ljbL23OPux1bp8YGhWtyKXzxRJQ
06RwPppCOAzJbLy2n0pUzfQK
-----END PRIVATE KEY-----)";

// Two clients for two different connections
WiFiClient wifiClientToThingsboard;
WiFiClientSecure wifiClientToMqtt;

PubSubClient mqttClient(wifiClientToMqtt);
ThingsBoard tb(wifiClientToThingsboard);

// function prototype
void connectToWiFi(const char * ssid, const char * pwd);
void generateRandomData();
void reconnect();

void callback(char* topic, byte* payload, unsigned int length);
void mqttTask(void* pvParameters);
bool mqttConnect();
bool parseData(String JSONData);


void setup() {
  Serial.begin(115200);
  // Lakukan koneksi dengan WifiSecure
  wifiClientToMqtt.setCACert(server_cert);
  wifiClientToMqtt.setCertificate(client_cert);
  wifiClientToMqtt.setPrivateKey(client_key);

  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

    // Koneksi MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  if ( !tb.connected() ) {
    reconnect();
  }
  tb.loop();
}

// function to connect to WiFi
void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected successfully: ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // start mqtt task after wifi connected
  xTaskCreatePinnedToCore(mqttTask, "MQTTClient", 8192, NULL, 1, NULL, 1); 
}

// function to connect to MQTT Broker
bool mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(MQTT_BROKER);
  if (mqttClient.connect("ESP32_master_thingsboard")) {
      mqttClient.subscribe(MQTT_TOPIC);
      Serial.print("\nSubscribed to ");
      Serial.print(MQTT_TOPIC);
      // publish first message to MQTT Broker
      mqttClient.publish(MQTT_TOPIC, "Hello from master");
      return true;
  } else {
      return false; 
  }
}

// MQTT Task
void mqttTask(void* pvParameters) {
  while (true) {
  // memastikan mqtt client selalu terkoneksi
  if (!mqttClient.connected()) {
    if(!mqttConnect()) {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
      } 
      else {
        Serial.print("\nConnected to hive MQTT broker\n");
      }
  }
  // run mqtt client
  mqttClient.loop();
  }
}

// ThingsBoard reconnect function
void reconnect() {
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Berhasil terhubung!");
    }
    Serial.print("Menghubungkan ke ThingsBoard node ...");
    if ( tb.connect(THINGSBOARD_SERVER, THINGSBOARD_DEVICE_TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " [ retrying in 5 seconds]" );
      // Wait 5 detik
      delay( 5000 );
    }
  }
}

// callback for receiving MQTT Topic Message from Broker
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  Serial.print("Pesan Diterima: ");
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  if(parseData(message)){
    sendTelemetry();
  } 
}

// function to send data to ThingsBoard
void sendTelemetry() {
  if (tb.connected()){
    tb.sendTelemetryFloat("temperature", temperature);
    tb.sendTelemetryFloat("tds", tds);
    tb.sendTelemetryFloat("turbidity", turbidity);
  }
}

// function to parse JSON data
bool parseData(String JSONData) {
  // allocate the memory for the document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, JSONData);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  // Fetch values.
  temperature = doc["temperature"];
  tds = doc["tds"];
  turbidity = doc["turbidity"];

  return true;
}