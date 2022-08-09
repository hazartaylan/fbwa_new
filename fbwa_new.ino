#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
Servo aileron;  // create servo object to control a servo
Servo aileron2;
Servo elevator;
Servo rudder;
Servo esc;

MPU6050 mpu(Wire);
unsigned long timer = 0;
int servoPulseEle,servoPulseAil;
int ch1,ch2,ch3,ch4,ch5;
int ch1pulse,ch2pulse,ch3pulse,ch4pulse,ch5pulse;
void setup() {
  Serial.begin(115200);
  Wire.begin();
   pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);

  pinMode(10,INPUT);
  aileron.attach(23);
  //aileron2.attach(22);
  elevator.attach(20);
  rudder.attach(21);
 // esc.attach(14);
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
 // esc.writeMicroseconds(1000);
delay(5000);
}

void loop() {
  mpu.update();
  
  if((millis()-timer)>10){ // print data every 10ms
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY : ");
  Serial.print(mpu.getAngleY());
  Serial.print("\tZ : ");
  Serial.println(mpu.getAngleZ());
  timer = millis();  
  }
  ch1=pulseIn(5,HIGH,21000);
  ch2=pulseIn(6,HIGH,21000);
  ch3=pulseIn(8,HIGH,21000);
  ch4=pulseIn(9,HIGH,21000);
  ch5=pulseIn(10,HIGH,21000);
  ch1pulse=map(ch1,1088,1923,40,140);//aileron
  ch2pulse=map(ch2,1088,1923,25,155);//elevator
  ch3pulse=map(ch3,1088,1923,1100,2000);//Motor
  ch4pulse=map(ch4,1088,1923,155,25);//Rudder
  ch5pulse=map(ch5,1088,1923,0,4);//switch
Serial.println(ch1pulse);
Serial.println(ch2pulse);
Serial.println(ch3pulse);
Serial.println(ch4pulse);
Serial.println(ch5pulse);
 if(mpu.getAngleX()<0){
    aileron.write(90+((ch1pulse-90 )+(mpu.getAngleX())) );
           if((ch2pulse > 23 && ch2pulse < 157) && (ch4pulse > 23 && ch4pulse < 157)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((ch4pulse + ch2pulse) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseAil = ((ch4pulse + (180 - ch2pulse)) / 2); //Aileron servosu, Elevatorun ve ters cevrilmis aileronun surelerinin toplaminin yarisi
  elevator.write(servoPulseEle-(mpu.getAngleY())+(mpu.getAngleX())-(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseAil+(mpu.getAngleY())-(ch1pulse-90)); // Aileron Servo nesnesine bulunan degeri yaz.
  } 
    if(mpu.getAngleY()<0){
      if((ch2pulse > 23 && ch2pulse < 157) && (ch4pulse > 23 && ch4pulse < 157)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((ch4pulse + ch2pulse) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseAil = ((ch4pulse + (180 - ch2pulse)) / 2); //Aileron servosu, Elevatorun ve ters cevrilmis aileronun surelerinin toplaminin yarisi
  elevator.write(servoPulseEle-(mpu.getAngleY())+(mpu.getAngleX())-(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseAil+(mpu.getAngleY())-(ch1pulse-90)); // Aileron Servo nesnesine bulunan degeri yaz.
  } 
    
    }
    

    
    }else if(mpu.getAngleX()>0){
       
     aileron.write((ch1pulse)+((mpu.getAngleX())));
                if((ch2pulse > 23 && ch2pulse < 157) && (ch4pulse > 23 && ch4pulse < 157)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((ch4pulse + ch2pulse) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseAil = ((ch4pulse + (180 - ch2pulse)) / 2); //Aileron servosu, Elevatorun ve ters cevrilmis aileronun surelerinin toplaminin yarisi
  elevator.write(servoPulseEle-(mpu.getAngleY())-(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseAil+(mpu.getAngleY())+(mpu.getAngleX())-(ch1pulse-90)); // Aileron Servo nesnesine bulunan degeri yaz.
  
  }
  if(mpu.getAngleY()>0){
                  if((ch2pulse > 23 && ch2pulse < 157) && (ch4pulse > 23 && ch4pulse < 157)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((ch4pulse + ch2pulse) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseAil = ((ch4pulse + (180 - ch2pulse)) / 2); //Aileron servosu, Elevatorun ve ters cevrilmis aileronun surelerinin toplaminin yarisi
 elevator.write(servoPulseEle-(mpu.getAngleY())-(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseAil+(mpu.getAngleY())+(mpu.getAngleX())-(ch1pulse-90)); // Aileron Servo nesnesine bulunan degeri yaz.
  } 
    
    }
  
  
      }
           // esc.write(ch3pulse);

}
