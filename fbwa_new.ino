#include <Servo.h>
#include <EasyTransfer.h>
EasyTransfer ET;
#include "Wire.h"
#include <SD.h>
#include <SPI.h>
String sdcard_log;
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MPU6050_light.h>
Servo myservo1;
Servo myservo2;
float heading,lati,longi,ilk_basinc,headingDegrees,declinationAngle,irtifa;
Servo aileron, elevator,rudder,esc; 
MPU6050 mpu(Wire);
Adafruit_BME280 bme; // I2C

struct RECEIVE_DATA_STRUCTURE{
  int servo1val;
  int servo2val;
   int servo3val;
  int servo4val;
  int switch1;
};
int servoPulseEle,servoPulseRud;

RECEIVE_DATA_STRUCTURE txdata;
void setup(){
  Serial3.begin(57600);
  // Serial.begin(115200);
  Wire.begin();
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  }
     pinMode(5,INPUT);
   pinMode(6,INPUT);
   pinMode(8,INPUT);
   pinMode(9,INPUT);
   pinMode(10,INPUT);
   Serial.println(F("BME280 test"));
    unsigned status;  
    // default settings
    status = bme.begin(0x76);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    Serial.println("-- Default Test --");
  ilk_basinc=bme.readPressure() / 100.0F;
   Serial.println("card initialized.");
  byte status1 = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status1);
  while(status1!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
  ET.begin(details(txdata), &Serial3);
  // Servo
  
  aileron.attach(23);
  elevator.attach(20);
   rudder.attach(21);
  esc.attach(14);
    esc.writeMicroseconds(1000);
delay(5000);
}
void loop(){
    mpu.update();

  ch1=pulseIn(5,HIGH,21000);
  ch2=pulseIn(6,HIGH,21000);
  ch3=pulseIn(8,HIGH,21000);
  ch4=pulseIn(9,HIGH,21000);
  ch5=pulseIn(10,HIGH,21000);
  ch1pulse=map(ch1,1088,1923,40,140);//aileron
  ch2pulse=map(ch2,1088,1923,1,179);//elevator
  ch3pulse=map(ch3,1088,1923,1100,2000);//Motor
  ch4pulse=map(ch4,1088,1923,179,1);//Rudder
  ch5pulse=map(ch5,1088,1923,0,4);//switch
fbwa_for_xbee();



irtifa = 44330*(1-pow((bme.readPressure()/100.0F / ilk_basinc),(1/5.255)));
sd_card_log();
}
 void fbwa_for_xbee(){
  
  if(mpu.getAngleX()<0){
    aileron.write(90+((ch1pulse-90 )+((mpu.getAngleX()))) );
           if((txdata.servo2val > 0 && txdata.servo2val < 180) && (txdata.servo4val > 0 && txdata.servo4val < 180)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((txdata.servo4val  + txdata.servo2val) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseRud = ((txdata.servo4val  + (180 - txdata.servo2val)) / 2); //rudder servosu, Elevatorun ve ters cevrilmis rudder surelerinin toplaminin yarisi
  if(ch1pulse-92>0){
    elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)+(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)); // rudder Servo nesnesine bulunan degeri yaz.
    }else if(ch1pulse-92<0){
      elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)+(ch1pulse-90)); // rudder Servo nesnesine bulunan degeri yaz.
      }
  
  } 
    if(mpu.getAngleY()<0){
      if((txdata.servo2val > 0 && txdata.servo2val < 180) && (txdata.servo4val  > 1 && txdata.servo4val  < 180)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((txdata.servo4val  + txdata.servo2val) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseRud = ((txdata.servo4val  + (180 - txdata.servo2val)) / 2); //rudder servosu, Elevatorun ve ters cevrilmis rudderın surelerinin toplaminin yarisi
   if(ch1pulse-92>0){
    elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)+(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)); // rudder Servo nesnesine bulunan degeri yaz.
    }else if(ch1pulse-92<0){
      elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)+(ch1pulse-90)); // rudder Servo nesnesine bulunan degeri yaz.
      }else{
        
        
        }
  } 
    
    }
    

    
    }else if(mpu.getAngleX()>0){
       
     aileron.write((ch1pulse)+((mpu.getAngleX())));
                if((txdata.servo2val > 0 && txdata.servo2val < 180) && (txdata.servo4val > 0 && txdata.servo4val  < 180)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((txdata.servo4val  + txdata.servo2val) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseRud = ((txdata.servo4val  + (180 - txdata.servo2val)) / 2); //rudder servosu, Elevatorun ve ters cevrilmis rudderın surelerinin toplaminin yarisi
 if(txdata.servo1val-92>0){
    elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)+(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)); // rudder Servo nesnesine bulunan degeri yaz.
    }else if(txdata.servo1val-92<0){
      elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)+(ch1pulse-90)); // rudder Servo nesnesine bulunan degeri yaz.
      }else{
        
        
        }
  
  }
  if(mpu.getAngleY()>0){
                  if((txdata.servo2val > 0 && txdata.servo2val < 180) && (txdata.servo4val > 0 && txdata.servo4val < 180)) //İki alici kanalindan da sinyal aliniyorsa
  {
  servoPulseEle = ((txdata.servo4val  + txdata.servo2val) / 2); //Elevator servosu, iki girisin surelerinin toplamının yapisi
  servoPulseRud = ((txdata.servo4val  + (180 - txdata.servo2val)) / 2); //rudder servosu, Elevatorun ve ters cevrilmis aileronun surelerinin toplaminin yarisi
 if(ch1pulse-92>0){
    elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)+(ch1pulse-90)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)); // rudder Servo nesnesine bulunan degeri yaz.
    }else if(ch1pulse-92<0){
      elevator.write(servoPulseEle+((mpu.getAngleY())/2)-((mpu.getAngleX())/2)); // Elevator Servo nesnesine bulunan degeri yaz.
  rudder.write(servoPulseRud-((mpu.getAngleY())/2)+(ch1pulse-90)); // rudder Servo nesnesine bulunan degeri yaz.
      }else{
        
        
        }
  } 
    
    }
  
  
      }
            esc.write(txdata.servo3val);
  }
  
void sd_card_log(){
 
  
  sdcard_log = String("TAKIM NO: ")  + "*"+String("316377")+ "*" +String("BME BASINC: ")  + "*"+String(bme.readPressure()/100.0F) 
  + "*" +String("İRTİFA:")  + "*"+
    String(irtifa) +String("AÇI X: ")  + "*"+String(mpu.getAccAngleX()) + "*"+String("AÇI Y: ")  + "*"+String(mpu.getAccAngleY()) +
    "*"+String("PUSULA:  ")  + "*"+String(heading)+"*"+String("CHANNEL 1: ")  + "*"+String(txdata.servo1val)+"*"+String("CHANNEL 2 ")  + "*"+String(txdata.servo2val)+"*"+
    String("CHANNEL 3: ")  + "*"+String(txdata.servo3val)+"*"+String("CHANNEL 4: ")  + "*"+String(txdata.servo4val);
       
  File dataFile = SD.open("log_airplane2.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(sdcard_log);
    dataFile.close();
    // print to the serial port too:
        Serial.println("SD CARD :");

    Serial.println(sdcard_log);
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }
  }
