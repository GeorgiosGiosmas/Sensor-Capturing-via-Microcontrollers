// Wire Slave Receiver
// by devyte
// based on the example by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// This example code is in the public domain.


#include <Wire.h>
#include <DHT11.h>

const int16_t I2C_SLAVE = 0xBB;

uint16_t received_temp_data = 0, received_potensiometer_data = 0;
byte data_id = 0;
bool new_temp_data = false, new_pot_data = false;

void setup() 
{
  Serial.begin(115200);                     // start serial for output
  Wire.begin(SDA, SCL, I2C_SLAVE);          // new syntax: join i2c bus (address required for slave)
  Wire.setClock(100000);
  Wire.onReceive(receiveEvent);             // register event
}

// Loop functions keeps sending the received values from STM32F407G Disc board to the Generic ESP8266 module..
void loop() 
{

  if(new_temp_data)
  {
    // Send the Temperature's sensor value.
    Serial.write(0x0);                                            // First send the id byte
    Serial.write((received_temp_data >> 8) & 0xFF);               // Send the upper byte 
    Serial.write(received_temp_data & 0xFF);                     // Send the lower byte

    new_temp_data = false;
  }

  delay(100);

  if(new_pot_data)
  {
    // Send the Potensiometer's value.
    Serial.write(0x1);                                           // First send the id byte
    Serial.write((received_potensiometer_data >> 8) & 0xFF);      // Send the upper byte 
    Serial.write(received_potensiometer_data & 0xFF);            // Send the lower byte

    new_pot_data = false;
  }

}

// Function that executes whenever data is received from master.
// This function is registered as an event, see setup().
void receiveEvent(size_t howmany) {

  (void)howmany;
  while (Wire.available() > 2) {  

    data_id = Wire.read();  

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
