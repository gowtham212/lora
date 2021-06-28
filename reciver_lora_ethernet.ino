#include <avr/wdt.h>
#include <SPI.h>
#include <LoRa.h>
#include <Ethernet.h>
#define ss 3
#define rst 9
#define dio0 2
String b1, b2, b3, b4, b5, b6, b7, b8;
int b9 = 0.0;
int b10 = 0.0;
int b11, b21, b31, b41, b51, b61, b71, b81, b91;
byte mac[] = { 0xAE, 0xAD, 0xDE, 0xEF, 0xEE, 0xED };

//// the rstahl router's dns address:
//IPAddress dnServer(172, 30, 29, 1);
//// the rstahl router's gateway address:
//IPAddress gateway(172, 30, 29, 1);
//// the rstahl subnet:
//IPAddress subnet(255, 255, 255, 0);
//// the rstahl ip address:
//IPAddress ip(172, 30, 29, 21);
//// the rstahl server address:
//char serv[] = "s-iot01.stahl.de";

IPAddress dnServer(172, 30, 29, 1);
// the router's gateway address:
IPAddress gateway(10, 10, 40, 1);
// the subnet:
IPAddress subnet(255, 255, 252, 0);
IPAddress ip(10, 10, 43, 15);
byte serv[] = {172, 16, 5, 233} ; //Enter the IPv4 address // inner ip bit


EthernetClient cliente;
byte counter = 0;

void(* resetFunc) (void) = 0;//declare reset function at address 0

void ethernet()
{
  Ethernet.init(10);  // Most Arduino shields

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
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  }

  else {
    Serial.print("DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to server");
}

void setup() {

  wdt_disable();  /* Disable the watchdog and wait for more than 2 seconds */
  delay(3000);  /* Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
  wdt_enable(WDTO_8S);  /* Enable the watchdog with a timeout of 2 seconds */
  Serial.begin(9600);
  pinMode(7, OUTPUT); //blue
  pinMode(6, OUTPUT); //red
  while (!Serial);
  Serial.println("LoRa Reciver");
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
    delay(500);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  ethernet();
}



void sender() {
  cliente.stop();
   //if (cliente.connect(serv, 81)) {           //rstahl Connecting at the IP address and port we saved before
   if (cliente.connect(serv, 80)) {             // bit Connecting at the IP address and port we saved before
    Serial.println("connected");
    //cliente.print("GET /iot/insert-folder/insert.php?");         //rstahl url
    cliente.print("GET /projects/iot/insert-folder/insert.php?");  //bit url
    cliente.print("temperature=");
    cliente.print(b2);
    cliente.print("&current=");
    cliente.print(b7);
    cliente.print("&humidity=");
    cliente.print(b4);
    cliente.print("&pressure=");
    cliente.print(b6);
    cliente.print("&dust=");
    cliente.print(b5);
    cliente.print("&lux=");
    cliente.print(b3);
    cliente.print("&pt100=");
    cliente.print(b1);
    cliente.print("&ktype=");
    cliente.print(b9);
    cliente.print("&ktype1=");
    cliente.print(b10);
    cliente.print("&pressure1=");
    cliente.println(b8);
    Serial.println("inserted");


    counter = 0;
    while (cliente.connected()) {
      if (cliente.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = cliente.read();
        Serial.print(c);
      }
    }

    Serial.println("disconnected");


  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    digitalWrite(6, HIGH);//red
    counter++;
    Serial.print("counter");
    Serial.println(counter);
    if (counter == 21) {
      delay(1000);
      resetFunc(); //call reset
      delay(1000);
      counter = 0;
    }
  }
}
void lora() {

  int packetSize = LoRa.parsePacket();
  //digitalWrite(6, LOW); //red
  if (packetSize) {
    // received a packet
    Serial.println("Received packet");

    // read packet
    while (LoRa.available()) {
      String a = LoRa.readString();
      Serial.println(a);
      int a1 = a.indexOf("a");
      int a2 = a.indexOf("b");
      int a3 = a.indexOf("c");
      int a4 = a.indexOf("d");
      int a5 = a.indexOf("e");
      int a6 = a.indexOf("f");
      int a7 = a.indexOf("g");
      int a8 = a.indexOf("h");
      int a9 = a.indexOf("i");
      int a10 = a.indexOf("j");
      int a11 = a.indexOf("k");
      int a12 = a.indexOf("l");
      int a13 = a.indexOf("m");
      int a14 = a.indexOf("n");
      int a15 = a.indexOf("o");
      int a16 = a.indexOf("p");



      b1 = a.substring(a1 + 1, a2);
      b2 = a.substring(a3 + 1, a4);
      b3 = a.substring(a5 + 1, a6);
      b4 = a.substring(a7 + 1, a8);
      b5 = a.substring(a9 + 1, a10);
      b6 = a.substring(a11 + 1, a12);
      b7 = a.substring(a13 + 1, a14);
      b8 = a.substring(a15 + 1, a16);


      Serial.print("pt100 = ");
      Serial.println(b1);
      Serial.print("dalls = ");
      Serial.println(b2);
      Serial.print("tsl   = ");
      Serial.println(b3);
      Serial.print("humi  = ");
      Serial.println(b4);
      Serial.print("dust  = ");
      Serial.println(b5);
      Serial.print("bme280   = ");
      Serial.println(b6);
      Serial.print("ct    = ");
      Serial.println(b7);
      Serial.print("bme680  = ");
      Serial.println(b8);
      Serial.print("ktype  = ");
      Serial.println(b9);
      Serial.print("ktype1  = ");
      Serial.println(b10);
      digitalWrite(7, HIGH); //blue
      delay(100);
      digitalWrite(7, LOW); //blue
      digitalWrite(6, LOW); //red

    }
    sender();
  }

}
void loop() {
  lora();
  Ethernet.maintain();
  wdt_reset(); // reset the wdt
  delay(100);

}


