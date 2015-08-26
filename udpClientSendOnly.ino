#include <EtherCard.h>

static byte mymac[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F };
byte Ethernet::buffer[700];
static uint32_t timer = 0x80000000;

const char website[] PROGMEM = "server.example.net";
const int dstPort PROGMEM = 37001;

const int srcPort PROGMEM = 37000;

void setup () {
  Serial.begin(115200);

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
  ether.printIp("BC:  ", ether.broadcastip);  


  //if (!ether.dnsLookup(website))
  //  Serial.println("DNS failed");
    
  ether.printIp("SRV: ", ether.broadcastip);
}

char textToSend[] = "test 123";
char buffer[256];
int writeIndex = 0;

bool readyToSend()
{
  bool rts = false;

  if (writeIndex == 256)
    rts = true;
  if ((millis() > timer) && (writeIndex > 0))
    rts = true;
    
  return rts;
}

void loop () {    
    while (! readyToSend())
    {
      if (writeIndex == 0)
      {
        timer = millis() + 1000;
      }
      if (Serial.available())
      {
        buffer[writeIndex]=Serial.read();
        Serial.print(buffer[writeIndex]);
        writeIndex++;
      }
    }
   timer = millis() + 1000;
   Serial.println("sending");
   ether.sendUdp(buffer, writeIndex, srcPort, ether.broadcastip, dstPort );   
   writeIndex=0;
}
