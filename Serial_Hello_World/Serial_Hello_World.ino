/*
* send "Hello World" over serial
*/

void setup(){
  Serial.begin(9600);
  Serial.println("Hello World");
}

void loop(){
  Serial.println("Hello!");
  delay(1000); // wait 1000 ms
}


