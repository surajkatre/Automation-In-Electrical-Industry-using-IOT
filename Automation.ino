#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "ThingSpeak.h"
#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"

/*
    DC Voltmeter Using a Voltage Divider

*/
byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddressip(192, 168, 0, 177);
IPAddressmyDns(192, 168, 2, 1);
EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
constintcurrentPin = A0;
int sensitivity = 66;
intadcValue = 0;
intoffsetVoltage = 2500;
doubleadcVoltage = 0;
doublecurrentValue = 0;

intanalogInput = A1;
floatvout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //
float R2 = 7500.0; //
int value = 0;
void setup() {
Ethernet.init(10);  // Most Arduino Ethernet hardware
Serial.begin(115200);  //Initialize serial
initEithernet();
  // Initialize ThingSpeak
pinMode(analogInput, INPUT);
lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("  DC Voltmeter ");
lcd.setCursor(0, 1);
lcd.print("TS Client Init");
lcd.print("              ");
}
Void init Eithernet() {
  // start the Ethernet connection:
Serial.println("Initialize Ethernet with DHCP:");
if (Ethernet.begin(mac) == 0) {
Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
if (Ethernet.hardwareStatus() == EthernetNoHardware) {
Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
while (true) {
delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
if (Ethernet.linkStatus() == LinkOFF) {
Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
Ethernet.begin(mac, ip, myDns);
  } else {
Serial.print("DHCP assigned IP ");
Serial.println(Ethernet.localIP());
lcd.setCursor(0, 1);
lcd.print("IP:");
lcd.setCursor(0, 1);
lcd.print(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
delay(1000);
ThingSpeak.begin(client);
}
void loop() {

  // read the value at analog input
value = analogRead(analogInput);
vout = (value * 5.0) / 1024.0; // see text
vin = vout / (R2 / (R1 + R2));

adcValue = analogRead(currentPin);
adcVoltage = (adcValue / 1024.0) * 5000;
currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
lcd.setCursor(0, 1);
lcd.print("V :");
lcd.print(vin);
lcd.print(" C:");
lcd.print(currentValue);
  // set the fields with the values
ThingSpeak.setField(1, String(vin));
ThingSpeak.setField(2, String(currentValue));

  // write to the ThingSpeak channel
int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println(x);
if (x == 200) {
Serial.println("Channel update successful.");
  }
else {
Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
delay(20000); // Wait 20 seconds to update the channel again

}

// Use this file to store all of the private credentials 
// and connection details

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
#define SECRET_MAC {0x90, 0xA2, 0xDA, 0x10, 0x40, 0x4F}

#define SECRET_CH_ID 712747			// replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "QI7K63VOHUOUSU5L"   // replace XYZ with your channel write API Key





