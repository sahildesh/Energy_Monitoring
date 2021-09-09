int status1 = 1 ;
int status2 = 1 ;
char state;
int val=0;

void setup(){
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);
}


void loop() {
  val = analogRead(23);
  Serial.println(val);
  //Serial.println(10);
  //Serial.write(45);
  if (Serial.available()){
  state = Serial.read();
  
    if (state == '1'){
      if(status1==0){
        status1=1;
        digitalWrite(8, HIGH);
        }
      else
       {
        status1=0;
        digitalWrite(8, LOW);
       } 
     }

    if (state == '2'){
      if(status2==0){
        status2=1;
        digitalWrite(9, HIGH);
        }
      else
       {
        status2=0;
        digitalWrite(9, LOW);
       } 
     }
     
  }
  delay(1000);
}
