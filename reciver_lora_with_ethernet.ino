//this id done by gowthamg212@gmail.com
#include <SPI.h>
#include <LoRa.h>
#define ss 3
#define rst 9
#define dio0 2
String b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
int b11, b21, b31, b41, b51, b61, b71, b81, b91;
#include <Ethernet.h>
byte mac[] = { 0xAE, 0xAD, 0xDE, 0xEF, 0xEE, 0xED };
IPAddress dnServer(192, 168, 30, 202);
// the router's gateway address:
IPAddress gateway(10, 10, 40, 1);
// the subnet:
IPAddress subnet(255, 255, 252, 0);
IPAddress ip(10, 10, 43, 12);
byte serv[] = {121, 200, 55, 62} ; //Enter the IPv4 address //outer ip bit
//byte serv[] = {172, 16, 5, 233} ; //Enter the IPv4 address // inner ip bit
//byte serv[] = {10, 10, 110, 32} ; //Enter the IPv4 address // local host
EthernetClient cliente;
int counter = 0;
void(* resetFunc) (void) = 0;
void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT); //red
  pinMode(6, OUTPUT); //blue

  while (!Serial);
  Serial.println("LoRa Reciver");

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
  Ethernet.begin(mac, ip, dnServer, gateway);
  //Ethernet.begin(mac);
  //  //print out the IP address
  Serial.print("IP = ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  int packetSize = LoRa.parsePacket();
digitalWrite(6, LOW); //blue
  if (packetSize) {
    // received a packet
    Serial.println("Received packet '");

    // read packet
    while (LoRa.available()) {
      String a = LoRa.readString();
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
      int a17 = a.indexOf("q");
      int a18 = a.indexOf("r");
      int a19 = a.indexOf("s");
      int a20 = a.indexOf("t");


      b1 = a.substring(a1 + 1, a2);
      b2 = a.substring(a3 + 1, a4);
      b3 = a.substring(a5 + 1, a6);
      b4 = a.substring(a7 + 1, a8);
      b5 = a.substring(a9 + 1, a10);
      b6 = a.substring(a11 + 1, a12);
      b7 = a.substring(a13 + 1, a14);
      b8 = a.substring(a15 + 1, a16);
      b9 = a.substring(a17 + 1, a18);
      b10 = a.substring(a19 + 1, a20);

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
      Serial.print("ktype = ");
      Serial.println(b8);
      Serial.print("ktyp1 = ");
      Serial.println(b9);
      Serial.print("bme680  = ");
      Serial.println(b10);
      digitalWrite(6, HIGH); //blue
      digitalWrite(5, LOW); //red


      if (cliente.connect(serv, 80)) { //Connecting at the IP address and port we saved before
        Serial.println("connected");
        //-------------------------------------
        //sending to ragav's local server - (10.10.110.32)
        //cliente.print("GET /rstahl/copies-for-sql-server/insert.php?"); //Connecting and Sending values to database
        //-------------------------------------
        //sending to ADC's server - (172.16.5.233)
        cliente.print("GET /projects/rstahl/copies-for-sql-server/insert.php?"); //Connecting and Sending values to database
        //-------------------------------------
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
        cliente.print(b8);
        cliente.print("&ktype1=");
        cliente.print(b9);
        cliente.print("&pressure1=");
        cliente.println(b10);
        //cliente.println();
        Serial.println("inserted");
        while (cliente.connected()) {
          if (cliente.available()) {
            // read an incoming byte from the server and print it to serial monitor:
            char c = cliente.read();
            Serial.print(c);
          }
        }
        Serial.println();
        counter = 0;
        cliente.stop();
        Serial.println("disconnected");

      }
      else {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
        digitalWrite(5, HIGH);
        Serial.print("counter:");
        counter++;
        Serial.println(counter);
        if (counter == 10) {
          resetFunc();
        }

      }


      delay(3000);

    }

  }


}  
