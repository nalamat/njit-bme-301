void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read analog value and convert it to units of mV
  int val = analogRead(A0) / 1023. * 5000;
  
  // Send out the read value over serial port to PC
  Serial.println(val);
}
