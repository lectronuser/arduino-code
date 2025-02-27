#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId = 0x03;
long unsigned int txId = 0x03;
unsigned char len = 5;
unsigned char rxBuf[8];
char msgString[128];                       

#define CAN0_INT 7                              
MCP_CAN CAN0(53);                        

byte uart_data[5] = {0x5A, 0x05, 0x45, 0x01, 0xA5};
byte save_data[4] = {0x5A, 0x04, 0x11, 0x6F};
byte data[1] = {0x5A};

void setup()
{
  Serial.begin(115200);
  if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     
  pinMode(CAN0_INT, INPUT);                    

}

void loop()
{
  receiverData();  
  delay(100);
}

void sendData(byte array, int size)
{
  byte sndStat = CAN0.sendMsgBuf(txId, 0, size, array);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100);   
}

void receiverData()
{
  if (CAN0.checkReceive() == CAN_MSGAVAIL)  
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);   

    if ((rxId & 0x80000000) == 0x80000000)  
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

    Serial.print(msgString);

    if ((rxId & 0x40000000) == 0x40000000)  
    {
      Serial.println(" REMOTE REQUEST FRAME");
    }
    else
    {
      for (byte i = 0; i < len; i++)
      {
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
      Serial.println(); 
    }
  }
}
