
 
#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 6
#define TxD 7
#include <math.h>
int a;
float temperature;
int B=3975;                  //B value of the thermistor
float resistance; 
SoftwareSerial blueToothSerial(RxD,TxD);
const int pingPin = 8;
const float pi = 3.1416 ;


float getTemp(){
 a=analogRead(0);
  resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
  //delay(1000);
  //Serial.print("Current temperature is ");
  Serial.println(temperature);
  return temperature;
}


void setup() 
{ 
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection(); 
} 

long getCm() {
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = duration / 29 / 2;
  Serial.println(cm);
  return cm;
}
 
void loop() 
{ 
  char recvChar;
  while(1){
    if(blueToothSerial.available()){//check if there's any data sent from the remote bluetooth shield
      recvChar = blueToothSerial.read();
      Serial.print(recvChar);
      
      if (recvChar == 'a')  {
         digitalWrite(13, HIGH);
      }
      
      if (recvChar == 'b')  {
         digitalWrite(13, LOW);      
      }
      
      if (recvChar == 'c')  {
         blueToothSerial.print(getTemp());
         blueToothSerial.print(",");
         float volume = pi * 5 * 5 * (13-getCm());
       //  blueToothSerial.println(volume);         
         blueToothSerial.println(volume);

      }

      if (recvChar == 'd')  {
         blueToothSerial.print(getTemp());
         blueToothSerial.print(",");
         float volume = x * x * (13-getCm());
       //  blueToothSerial.println(volume);         
         blueToothSerial.println(volume);
      }

      
    }
    if(Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
      recvChar  = Serial.read();
      blueToothSerial.print(recvChar);
    }
  }
} 
 
void setupBlueToothConnection()
{
  blueToothSerial.begin(115200); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=AquaMed\r\n"); //set the bluetooth name as "SeeedBTSlave"
  blueToothSerial.print("\r\n+STPIN=0000\r\n");//Set SLAVE pincode"0000"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}




