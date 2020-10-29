#include <SPI.h>
#include <LoRa.h>
#define ss 10
#define rst 9
#define dio0 2

#include <Wire.h>
#include "DFRobot_BME280.h"
typedef DFRobot_BME280_IIC    BME;    // ******** use abbreviations instead of full names ********
BME   bme1(&Wire,0x76);   // select TwoWire peripheral and set sensor address
#define SEA_LEVEL_PRESSURE    1015.0f
float press=0.0;
#include "uFire_SHT20.h"
uFire_SHT20 sht20;
float humid = 0.0;
float humid1 = 0.0;

#include "DFRobot_BME680_I2C.h"
#include "Wire.h"
#define CALIBRATE_PRESSURE
DFRobot_BME680_I2C bme(0x77);  //0x77 I2C address
float seaLevel;
float P02=0.0;

#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
String u = "";
float u1,u2;


#include <Adafruit_AM2315.h>
Adafruit_AM2315 am2315;
float z=0.0;

#include<DFRobotHighTemperatureSensor.h>
const float voltageRef = 5.0;
int HighTemperaturePin = A1;  //Setting pin
DFRobotHighTemperature PT100 = DFRobotHighTemperature(voltageRef);
float pt=0.0;
#include <SDS011.h>
float p10, p25;
int error;
SDS011 my_sds;

#include "EmonLib.h"
EnergyMonitor emon1;
float y;

#define TC_PIN A2
#define AREF 5.0
#define ADC_RESOLUTION 10
float reading, voltage, temperatures,temperatures1;
float get_voltage(int raw_adc) {
  return raw_adc * (AREF / (pow(2, ADC_RESOLUTION) - 1));
}
float get_temperature(float voltage) {
  return (voltage - 1.25) / 0.005;
}


#define TC_PIN1 A3
#define AREF1 5.0
#define ADC_RESOLUTION1 10  // set to ADC bit resolution, 10 is default
float reading1, voltage1, temperatures2,temperatures3;
float get_voltage1(int raw_adc1) {
  return raw_adc1 * (AREF1 / (pow(2, ADC_RESOLUTION1) - 1));
}
float get_temperature1(float voltage1) {
  return (voltage1 - 1.25) / 0.005;
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  //  Serial.println(F("------------------------------------"));
  //  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  //  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  //  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  //  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  //  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  //  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));
  //  Serial.println(F("------------------------------------"));
  //  Serial.println(F(""));
  delay(100);
}


void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */
  //  Serial.println(F("------------------------------------"));
  //  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));        
      break;
  }
  //  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println(F(" ms"));
  //  Serial.println(F("------------------------------------"));
  //  Serial.println(F(""));
}
void setup()
{
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  my_sds.begin(5,6);
  bme1.begin();
  am2315.begin();// Join i2c bus
 

  emon1.current(0, 80.0);//A0 for current,A1 for pt100

  Serial.print(bme.begin());
  #ifdef CALIBRATE_PRESSURE
  bme.startConvert();
  bme.update();
  seaLevel = bme.readSeaLevel(525.0);
 
  #endif

  if (tsl.begin())
  {
    Serial.println(F("Found a TSL2591 sensor"));
  }
  displaySensorDetails();
  configureSensor();

 while (!Serial);
  Serial.println("LoRa sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
 
  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(867E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setTxPower(14, 148);

  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop()
{
  Serial.println("Sending packet: ");
  LoRa.beginPacket();
  advancedRead();
 
 
  Serial.print("bme280 press");
  press = (bme1.getPressure());
  Serial.println(press);


  humid = sht20.humidity();
  Serial.print("SHT20 humidity: ");
  humid =((humid-8)*1.04);
  if(humid > 100.00){
    humid = 0.0;
  }
 
  Serial.print(humid);
  Serial.println();

 
 float temperature5, humidity5;
 float z= (am2315.readTemperatureAndHumidity(&temperature5, &humidity5));
 Serial.print("am315: ");
 if((temperature5==516.27)||(temperature5==-25225231000000000000.00))
{
  temperature5=0.0;
  }
 Serial.println(temperature5);


  float temperature = PT100.readTemperature(HighTemperaturePin);  //Get temperature
  Serial.print("pt100 temperature: ");
  pt=((temperature)/1.385);
  if(pt<4.0){
    pt =0.0;
  }
  Serial.print(pt);
  Serial.println("C");
 
 
  bme.startConvert();
  bme.update();
  Serial.print("bme pressure(Pa) :");
  P02=(bme.readPressure());
  if(P02>129467300.000){
    P02=0.0;
    }
  Serial.println(P02);
  #ifdef CALIBRATE_PRESSURE
  #endif

 
  reading = analogRead(TC_PIN);
  voltage = get_voltage(reading);
  temperatures = (get_temperature(voltage)*0.35);
  Serial.print("kype Temperature: ");
   if((temperatures<0.0)||(temperatures==262.50)){
    temperatures = 0.0;
  }
  Serial.print(temperatures);
  Serial.println("C");

  reading1 = analogRead(TC_PIN1);
  voltage1 = get_voltage1(reading1);
  temperatures2 = (get_temperature1(voltage1)*0.35);
  Serial.print("kype1 Temperature");
 
   if((temperatures2<0.0)||(temperatures2==262.50)){
    temperatures2 = 0.0;
 }
  Serial.print(temperatures2);
  Serial.println("C");

   unsigned long previousMillis = millis();
  int count = 0;
  double Irms = 0;
  while ((millis() - previousMillis) < 1000)
  {
    Irms += emon1.calcIrms(1480);  // Calculate Irms only
    count++;
  }
  Irms = Irms / count;
  y = ((Irms * 230.0) / 1000.0);
  Serial.print("ct");
  y= (y*1.054);
  if(y<0.12){
    y=0.0;}
  Serial.println(y);      
 
   error = my_sds.read(&p25, &p10);
  if (! error) {
    Serial.println("P2.5: " + String(p25));
    Serial.println("P10:  " + String(p10));
  }
  delay(500);

  String sensor_data = "a" + String(pt) + "b" + "c" + String(temperature5 ) + "d" + "e" +String(u2) + "f" + "g" + String(humid) + "h" + "i" + String(p25) + "j" + "k" + String(press) + "l" + "m" + String(y) + "n" + "o" + String(temperatures) + "p" + "q" + String(temperatures2) + "r"+"s"+String(P02)+"t";
  Serial.println(sensor_data);
  LoRa.print(sensor_data);
 
  LoRa.endPacket();
  delay(500);

}
void simpleRead(void)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

//  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
//  Serial.print(F("Luminosity: "));
//  Serial.println(x, DEC);
}

/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  //  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  //  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  //  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  //  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  u = String(tsl.calculateLux(full, ir));
  if(u==" nan" || u==" NAN"|| u==" inf" || u==" INF" || u=="INF")
  {
  u2=0.0;
    Serial.print(F("TSl2591 old  Lux: "));
    Serial.println(u);
  }
    else
    {
  u1=((4.0*u.toFloat()));
  if(u1<0)
  u2=0.0;
  else
  u2=u1;
    Serial.print(F("TSl2591 new  Lux: "));
   Serial.println(u2);            
    }
   
//  Serial.println(tsl.calculateLux(full, ir),6);
}

void unifiedSensorAPIRead(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  tsl.getEvent(&event);

  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) |
      (event.light < -4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Invalid data (adjust gain or timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}
