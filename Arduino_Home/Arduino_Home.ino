void Nahoru(int i)
{   if(i == 1){
    for(int k = 0;k <= 6;k++){
        digitalWrite(k,HIGH);
        delay(200);
    }
   }
    if(i == 0){
    for(int k = 0;k >= 6;k--){
        digitalWrite(k,LOW);
        delay(500);
    }
   }
}

void Dolu(int q)
{   if(q == 1){
    for(int k = 0;k >= 6;k--){
        digitalWrite(k,HIGH);
        delay(200);
    }
   }
    if(q == 0){
    for(int k = 0;k <= 6;k++){
        digitalWrite(k,LOW);
        delay(500);
    }
   }
}

void setup(){
  Serial.begin(9600);  
  for(int i = 0;i<=6;i++){
      pinMode(i,OUTPUT);
    }
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    
}


void loop(){
    int lol = digitalRead(7);
    int lol2 = digitalRead(8);
    Serial.println(digitalRead(7));
    Serial.println(digitalRead(8));

    if(lol == HIGH){
    Dolu(1);
    delay(5000);
    Dolu(0);
    } 
  
    if(lol2 == HIGH){
    Nahoru(1);
    delay(5000);
    Nahoru(0);
    }
}
