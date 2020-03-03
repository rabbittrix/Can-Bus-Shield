// ****** INCLUDES ******
#include <SPI.h>
#include <mcp_can.h>
#include <LiquidCrystal.h>


// ****** GLOBALS ******
const int SPI_CS_PIN = 9;
int pushButton = 8;
int pot_lcd = 0;
int pot_can = 0;
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int cntr;
int buttonState;  

MCP_CAN CAN(SPI_CS_PIN);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// ****** STANDARD ARDUINO SETUP FUNCTION ******
void setup() {

  Serial.begin(9600);
  lcd.begin(20, 4);     // init display
  lcd.print("Initialized");   // print initial message on LCD display

  // CAN shield initialization : baudrate = 500k
  while (CAN_OK != CAN.begin(CAN_500KBPS))           
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println("Init CAN BUS Shield again");
      delay(100);
  }

  Serial.println("CAN BUS Shield init ok!");

}

// ****** STANDARD ARDUINO LOOP FUNCTION ******
void loop() {

  // VARIABLES
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long canId = 0;

  // RECEIVING CAN MESSAGES
  if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
  {
    CAN.readMsgBuf(&len, buf);    // read incoming data
    canId = CAN.getCanId();

    // check for push button status message (70)
    if (canId == 71) {

      pot_can = (buf[1] * 256) + buf[0];
      SetLCD();
      
    }
    
  }

  delay(50);

  // SENDING CAN MESSAGES
  cntr = cntr + 1;
  if (cntr > 10)  {
    cntr = 0;

    buttonState = digitalRead(pushButton);
    if (buttonState == 0) {
      stmp[0] = 0;
    }     
    else  {
      stmp[0] = 1;
    }

    CAN.sendMsgBuf(70, 0, 8, stmp);
    
  } 

}

// ****** SUPPORT FUNCTIONS ******
void SetLCD() {

  String tmpstr;
  
  if (pot_lcd != pot_can) {
    
    lcd.clear();        // clear display

    tmpstr = "Pot Value: " + String(pot_can);
    lcd.setCursor(0,0);
    lcd.print(tmpstr);

    pot_lcd = pot_can;
    
  }
  
}








