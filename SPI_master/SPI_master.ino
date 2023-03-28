#include <SPI.h>

/* Cosmic Pi SPI test routines - Master version, slave is commented out below. Runs on an Arduino DUE,
 *  Pinouts:
 *  MOSI - MOSI
 *  MISO - MISO
 *  SCK - SCK
 *  Pin 10 - Pin 10 (SS)
 *  GND - GND
 *  5V - 5V
 *  
 *  The slave echoes the master byte in the next (16 bit) transmission. 
SPI COMMUNICATION TRANSACTION LIST OF COMMANDS
WRITE OPERATIONS (FOR EACH WRITE, SLAVE MUST SEND BACK THE VALUE IT RECEIVES)
01  WRITE MSB(16 high bits) HV1_DUTY_CYCLE
02  WRITE LSB(16 low bits) HV1_DUTY_CYCLE
03  WRITE MSB(16 high bits) HV2_DUTY_CYCLE
04  WRITE LSB(16 low bits) HV2_DUTY_CYCLE
05  WRITE 16 bits LED_DUTY_CYCLE
READ OPERATIONS (FOR EACH READ, SLAVE MUST SEND BACK THE VALUE OF THE RELATED REGISTER)
06  READ MSB(16 high bits) HV1_DUTY_CYCLE
07  READ LSB(16 low bits) HV1_DUTY_CYCLE
08  READ MSB(16 high bits) HV2_DUTY_CYCLE
09  READ LSB(16 low bits) HV2_DUTY_CYCLE
10  READ 16 bits LED_DUTY_CYCLE
11  WRITE STATUS
11  READ STATUS
*/

byte CMD=1;

unsigned int duty_cycle_LED = 81;
unsigned int duty_cycle_channel_A = 100000;
unsigned int duty_cycle_channel_B = 200000;
unsigned int MSB;
unsigned int LSB;

//unsigned int response = 0;
unsigned int answer = 0;
byte PSU_parametrized = 0;

void setup() {

pinMode(SS, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(SPI_CLOCK_DIV128 );
  Serial.begin(9600);
  
  //delay(500);

}


void loop() {




      // WRITE OPERATIONS
      Serial.println("*******************  WRTTE  OPERATIONS  *************************");

      /********************* HV1 DUTY CYCLE *********************************/

 
      delay(20);

      //MSB
      MSB = duty_cycle_channel_A >> 16;
      spi_exchange(1);
      spi_exchange(MSB);

      //LSB
      LSB = duty_cycle_channel_A & 0x0000FFFF;
      spi_exchange(1);
      spi_exchange(LSB);

      /********************* HV2 DUTY CYCLE *********************************/
      //MSB
      MSB = duty_cycle_channel_B >> 16;
      spi_exchange(3);
      spi_exchange(MSB);

      //LSB
      LSB = duty_cycle_channel_B & 0x0000FFFF;
      spi_exchange(4);
      spi_exchange(LSB);


      /********************* LED DUTY CYCLE *********************************/
      spi_exchange(5);
      spi_exchange(duty_cycle_LED);

      // READ OPERATIONS
      Serial.println("*******************  READ  OPERATIONS  *************************");
      
      /********************* HV1 DUTY CYCLE *********************************/
      //MSB
      spi_exchange(6);
      spi_exchange(255);

      //LSB
      CMD=7;
      spi_exchange(CMD);
      spi_exchange(255);


      /********************* HV2 DUTY CYCLE *********************************/
      //MSB
      CMD=8;
      spi_exchange(CMD);
      spi_exchange(255);

      //LSB
      CMD=9;
      spi_exchange(CMD);
      spi_exchange(255);

      /********************* LED DUTY CYCLE *********************************/
      CMD=10;
      spi_exchange(CMD);
      spi_exchange(255);

      /********* STATUS *******/
      // WRITE
      CMD=11;
      spi_exchange(CMD);
      spi_exchange(11);

      // READ
      CMD=12;
      spi_exchange(CMD);
      spi_exchange(255);

  
      delay(4000);


}

int spi_exchange(int data){
      digitalWrite(SS, LOW);
      int response = SPI.transfer16(data);
      Serial.println("Master send command:" + String(data) + " Slave reply:" + String(response));
      digitalWrite(SS, HIGH);
      delay(20);
      return response;
}
