byte mcp3912_cs = 13;
byte mcp3912_dr = 12;
byte mcp3912_rst = 5;

byte mcp3912_miso = 19;
byte mcp3912_mosi = 23;
byte mcp3912_sck = 18;
int start_bit = 0xA0;
int end_bit = 0xC0;
int garbage_data[8] = {0xF1F2F3,0xF1F2F3,0xF1F2F3,0xF1F2F3,0xF1F2F3,0xF1F2F3,0xF1F2F3,0xF1F2F3};
uint8_t i=0,j=0;
float t;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
pinMode(mcp3912_sck, OUTPUT);
pinMode(mcp3912_miso, OUTPUT);
pinMode(mcp3912_mosi, OUTPUT);
pinMode(mcp3912_cs, OUTPUT);
pinMode(mcp3912_dr, OUTPUT);
pinMode(mcp3912_rst, OUTPUT);

 char input = Serial.read();
 if (input == 'v')
  Serial.print("0x1234,0x9876$$$");
 else
 delay(3000);
 Serial.print("0x1234,0x9876$$$");
 
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(mcp3912_cs, 1);
//  delay(500);
//  digitalWrite(mcp3912_cs, 0);
//  delay(500);
  if ('s' != Serial.read()){
  Serial.print(start_bit);
  Serial.print(i);
//  for (j=0;j<8;j++)
  t=millis()/1.0e6;
  Serial.print(sin(2*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(3*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(4*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(5*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(6*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(7*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(8*PI*2*t));
  t=millis()/1.0e6;
  Serial.print(sin(9*PI*2*t));
  Serial.print(end_bit);
  if (i>255)
  i=0;
  else 
  i++;
  }
  else
  i=0;
  
//  Serial.println("45997.52, 37487.45, -22369.71, -26183.10, -12716.55, -24777.78, -10055.49, -3746.69");
}
