int pbuttonPin1 = 2;// connect output to push button
int pbuttonPin2 = 5;
int relayPin1 = 8;// Connected to relay (LED)
int relayPin2 = 9;

int val_1 = 0; // push value from pin 2
int val_2 = 0; // push value from pin 2
int lightON_1 = 0;//light status
int pushed_1 = 0;//push status
int lightON_2 = 0;//light status
int pushed_2 = 0;//push status

const int sensorIn = 23;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module


double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;






void setup(){ 
 Serial.begin(9600);
  pinMode(pbuttonPin1, INPUT_PULLUP); 
  pinMode(pbuttonPin2, INPUT_PULLUP); 
  pinMode(relayPin1, OUTPUT);
  digitalWrite(relayPin1, LOW);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, LOW);
}

void loop(){

 val_1 = digitalRead(pbuttonPin1);// read the push button value
  //Serial.println(val_1);
  if(val_1 == HIGH && lightON_1 == LOW){

    pushed_1 = 1-pushed_1;
    delay(100);
  }    

  lightON_1 = val_1;

      if(pushed_1 == HIGH){
        digitalWrite(relayPin1, HIGH); 
       
      }else{
        digitalWrite(relayPin1, LOW);
      }     
//--------------------------------------------------------------------------------------------------------------------------------
  val_2 = digitalRead(pbuttonPin2);// read the push button value
  //Serial.println(val_2);
  if(val_2 == HIGH && lightON_2 == LOW){

    pushed_2 = 1-pushed_2;
    delay(100);
  }    

  lightON_2 = val_2;

      if(pushed_2 == HIGH){
        digitalWrite(relayPin2, HIGH); 
       
      }else{
        digitalWrite(relayPin2, LOW);
      }
 
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; 
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 AmpsRMS = (AmpsRMS-1.27)*0.6;


    Serial.print("\n Amps RMS :");
    Serial.println(AmpsRMS);


 delay(500); 

}

float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 4096;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 100) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       //Serial.println(readValue);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 3.3)/4096.0; 
   return result;
 }
