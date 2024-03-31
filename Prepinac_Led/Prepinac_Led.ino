int led_1 = 8;
int led_2 = 9;
int tlac = 7;
int pocet;

void setup() {
  // put your setup code here, to run once:
  pinMode (led_1, OUTPUT);
  pinMode (led_2, OUTPUT);
  pinMode (tlac, INPUT);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  pocet = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (digitalRead(tlac) == HIGH){
    pocet ++;

  }
  
  if (pocet % 2 == 0){
    digitalWrite(led_2, HIGH);
     }
  

  else{
    digitalWrite(led_2, LOW);
  }
  
}
