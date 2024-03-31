
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

struct Data_Package{
  int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
  int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
  int16_t temperature; // variables for temperature data
  };
  Data_Package data;

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    
    radio.read(&data, sizeof(Data_Package));
 
 //Výtisk hodnot
 
  Serial.print("aX = "); Serial.print(convert_int16_to_str(data.accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(data.accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(data.accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(data.temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(data.gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(data.gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(data.gyro_z));
  Serial.println();

 
    }
  }
