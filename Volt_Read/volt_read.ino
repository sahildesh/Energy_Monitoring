const int analogInPin = 24;
const int ref = 25;  
float sensorValue = 0;    
float refValue = 0; //DC level shift
int count=0;
float temp_rms=0;
float rms=0;
int avg_cnt=0;
float avg_rms=0;

float b = 0;
void setup() {
   Serial.begin(9600);
}

void loop() {
 
  sensorValue = analogRead(analogInPin);
  refValue = analogRead(ref);
  refValue = refValue*(3.3/4096);
  b=sensorValue*(3.3/4096);
  b=b-refValue; 
  b=b*267.66; //Scaling factor
  
  //Serial.print("\n sensor = ");
  //Serial.print(refValue);
  count++;
  temp_rms+=(b*b);
  
  if(count%100==0){
    count=0;
    avg_cnt++;
    rms=sqrt(temp_rms/100);
    avg_rms=avg_rms+rms;
    if(avg_cnt%40==0){
      avg_cnt=0;
       Serial.print("\n rms =");
       Serial.print(avg_rms/40);
       avg_rms=0;
    }
    temp_rms=0;
  }
  delay(2); //sampling rate = 100 times input frequency
}
