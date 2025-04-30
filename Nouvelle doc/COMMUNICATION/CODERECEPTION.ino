#include <SoftwareSerial.h>

// Define SoftwareSerial pins for communication with E32
#define E32_RX 10  // Arduino receives data from E32
#define E32_TX 11  // Arduino sends data to E32
#define M0 8       // Mode selection pin
#define M1 9       // Mode selection pin

SoftwareSerial e32Serial(E32_RX, E32_TX);  // RX, TX

void setup() {
  // Set pin modes for M0 and M1
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  // Set module to normal mode (M0 = LOW, M1 = LOW)
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  // Initialize serial communication
  Serial.begin(9600);      // Debug terminal
  e32Serial.begin(9600);   // E32 module communication

  Serial.println("E32 Module Test");
}

void loop() {
  // Send data to E32
  if (Serial.available()) {
    String data = Serial.readString();
    e32Serial.print(data);  // Send to E32
    Serial.println("Sent to E32: " + data);
  }                     
  // Receive data from E32                            
  if (e32Serial.available()) {                                                                                                                                                                                                         
    String received = e32Serial.readString();
    Serial.println(receivedData);
  }
}
