/*tak mno prvni pokus */
int seg1 = 1;
int seg2 = 2;
int seg3 = 3;
int seg4 = 4;
int seg5 = 5;
int seg6 = 6;
int seg7 = 7;
int seg8 = 8;//dot
void setup() {
  // put your setup code here, to run once:

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
 /*pinMode(9, OUTPUT);*/

}

void loop() {
  // put your main code here, to run repeatedly:
//0
  digitalWrite (seg1, HIGH);
  digitalWrite (seg2, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg5, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg2, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg5, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  delay (1000);
  //1
  digitalWrite (seg3, HIGH);
  digitalWrite (seg6, HIGH);
  delay (1000);
  digitalWrite (seg3, LOW);
  digitalWrite (seg6, LOW);
  delay (1000);
  //2
  digitalWrite (seg1, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg5, HIGH);
  digitalWrite (seg7, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg5, LOW);
  digitalWrite (seg7, LOW);
  delay (1000);
  //3
  digitalWrite (seg1, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  delay (1000);
 //4
  digitalWrite (seg2, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg6, HIGH);
  delay (1000);
  digitalWrite (seg2, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg6, LOW);
  delay (1000);
 //5
  digitalWrite (seg1, HIGH);
  digitalWrite (seg2, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg2, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  delay (1000);
  //6
  digitalWrite (seg1, HIGH);
  digitalWrite (seg2, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg5, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  digitalWrite (seg8, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg2, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg5, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  digitalWrite (seg8, LOW);
  delay (1000);
  //7
  digitalWrite (seg1, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg6, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg6, LOW);
  delay (1000);
  //8
  digitalWrite (seg1, HIGH);
  digitalWrite (seg2, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg5, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg2, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg5, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  delay (1000);
  //9
  digitalWrite (seg1, HIGH);
  digitalWrite (seg2, HIGH);
  digitalWrite (seg3, HIGH);
  digitalWrite (seg4, HIGH);
  digitalWrite (seg6, HIGH);
  digitalWrite (seg7, HIGH);
  digitalWrite (seg8, HIGH);
  delay (1000);
  digitalWrite (seg1, LOW);
  digitalWrite (seg2, LOW);
  digitalWrite (seg3, LOW);
  digitalWrite (seg4, LOW);
  digitalWrite (seg6, LOW);
  digitalWrite (seg7, LOW);
  digitalWrite (seg8, LOW);
  delay (1000);

}
