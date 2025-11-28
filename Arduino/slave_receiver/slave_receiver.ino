#include <Wire.h>

// Wire Slave Receiver
// by devyte
// based on the example by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// This example code is in the public domain.

#include <avr/wdt.h>
#include <Wire.h>
#include <DHT11.h>

#define LED_PWM_PIN  8

const uint8_t I2C_SLAVE = 0x3A;

uint16_t received_temp_data = 0, received_potensiometer_data = 0;
unsigned long last = 0;
byte data_id = 0;
bool new_temp_data = false, new_pot_data, new_dht11_data = false;
int dht11_temperature, dht11_humidity, result = 0;
uint8_t brightness = 0;
DHT11 dht11 = DHT11(4);

void setup() 
{
  Serial.begin(115200);                     // start serial for output
  Wire.begin(I2C_SLAVE);                    // new syntax: join i2c bus (address required for slave)
  Wire.setClock(100000);
  Wire.onReceive(receiveEvent);             // register event
  wdt_enable(WDTO_4S);                      // Enable watchdog (2-second timeout)
}

// Loop functions keeps sending the received values from STM32F407G Disc board to the Generic ESP8266 module..
void loop() 
{

  wdt_reset();

  if(new_temp_data)
  {

    new_temp_data = false;

    // Send the Temperature's sensor value.
    Serial.write(0x0);                                            // First send the id byte
    Serial.write((received_temp_data >> 8) & 0xFF);               // Send the upper byte 
    Serial.write(received_temp_data & 0xFF);                      // Send the lower byte
    
  }

  if(new_pot_data)
  {

    new_pot_data = false;

    // Send the Potensiometer's value.
    Serial.write(0x1);                                            // First send the id byte
    Serial.write((received_potensiometer_data >> 8) & 0xFF);      // Send the upper byte 
    Serial.write(received_potensiometer_data & 0xFF);             // Send the lower byte
    
    // Adjust the brightness of the local LED.
    brightness = ((received_potensiometer_data/100.0)/3.3)*255.0;
    analogWrite(LED_PWM_PIN, brightness);
  }

  if(new_dht11_data)
  {
    
    new_dht11_data = false;

    result = dht11.readTemperatureHumidity(dht11_temperature, dht11_humidity);

    // Send DHT11's Temperature value.
    Serial.write(0x2);                                            // First send the id byte
    Serial.write((dht11_temperature >> 8) & 0xFF);               // Send the upper byte 
    Serial.write(dht11_temperature & 0xFF);                     // Send the lower byte

    // Send DHT11's Humidity value.
    Serial.write(0x3);                                            // First send the id byte
    Serial.write((dht11_humidity >> 8) & 0xFF);               // Send the upper byte 
    Serial.write(dht11_humidity & 0xFF);                     // Send the lower byte
  }

  DHT11_new_data();

}

// Function that executes whenever data is received from master.
// This function is registered as an event, see setup().
void receiveEvent(size_t howmany) {
 
  if(howmany == 1)
  {
    data_id = Wire.read();  
  }

  if(howmany == 2)
  {
    if(data_id == 0)
    {

      received_temp_data = Wire.read();
      received_temp_data |= Wire.read() << 8;
      
      new_temp_data = true;

    } else if(data_id == 1)
    {
      
      received_potensiometer_data = Wire.read();
      received_potensiometer_data |= Wire.read() << 8;
    
      new_pot_data = true;
    }  
  }                 
}

void DHT11_new_data()
{
  if (millis() - last >= 3000) {
    last = millis();
    new_dht11_data = true;
  }
}


