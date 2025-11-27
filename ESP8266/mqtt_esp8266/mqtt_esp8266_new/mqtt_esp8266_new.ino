/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Giosmas";
const char* password = "20012004";
const char* mqtt_server = "54.36.178.49"; // test.mosquitto.org IP -> This is a test server.

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

uint16_t received_temp_data = 0, received_potensiometer_data = 0, received_dht11_temperature = 0, received_dht11_humidity = 0;
byte data_id = 0;
bool new_temp_data = false, new_pot_data = false, new_dht11_temp_data = false, new_dht11_humidity_data;
float temperature = 0.0, potensiometer = 0.0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("device/temperature", "MQTT Server is Connected");
      client.publish("device/potensiometer", "MQTT Server is Connected");
      client.publish("device/dht11", "MQTT Server is Connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(new_temp_data)
  {
    // Send the Temperature's sensor value to the MQTT server.
    temperature = received_temp_data / 100.0;
    snprintf (msg, MSG_BUFFER_SIZE, "Temperature is #%lf °C", temperature);
    client.publish("device/temperature", msg);

    new_temp_data = false;
  }

  delay(20);

  if(new_pot_data)
  {
    // Send the Potensiometer's value to the MQTT server.
    potensiometer = received_potensiometer_data / 100.0;
    snprintf (msg, MSG_BUFFER_SIZE, "Potensiometer's value is #%lf V", potensiometer);
    client.publish("device/potensiometer", msg);

    new_pot_data = false;
  }

  delay(20);

  if(new_dht11_temp_data)
  {
    // Send the DHT11's temperature to the MQTT server.
    snprintf (msg, MSG_BUFFER_SIZE, "The temperature from the DHT11 sensor is #%ld °C", received_dht11_temperature);
    client.publish("device/dht11", msg);

    new_dht11_temp_data = false;
  }

  delay(20);

  if(new_dht11_humidity_data)
  {
    // Send the Potensiometer's value to the MQTT server.
    snprintf (msg, MSG_BUFFER_SIZE, "The humidity from the DHT11 sensor is #%ld %%", received_dht11_humidity);
    client.publish("device/dht11", msg);

    new_dht11_humidity_data = false;
  }
}

void serialEvent()
{
  while(Serial.available() > 2)
  {
    data_id = Serial.read();  

    if(data_id == 0)
    {

      received_temp_data = Serial.read() << 8;
      received_temp_data |= Serial.read();

      new_temp_data = true;

    } else if(data_id == 1)
    {
      
      received_potensiometer_data = Serial.read() << 8;
      received_potensiometer_data |= Serial.read();

      new_pot_data = true;

    } else if(data_id == 2)
    {
      
      received_dht11_temperature = Serial.read() << 8;
      received_dht11_temperature |= Serial.read();

      new_dht11_temp_data = true;

    } else if(data_id == 3)
    {
      
      received_dht11_humidity = Serial.read() << 8;
      received_dht11_humidity |= Serial.read();

      new_dht11_humidity_data = true;
    }
  }
}
