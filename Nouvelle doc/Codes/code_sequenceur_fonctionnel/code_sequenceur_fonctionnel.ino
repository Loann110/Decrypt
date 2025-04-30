#define jack 2

//Led board setup
#define led_green A0
#define led_blue  A1
#define led_red   A2
//end

//Stepper motor setup
#define enable_stepper D7
#define step 3 //control the speed with PWM signal
#define direction_stepper // Left (0) and right (1)
//end

//DC motor setup
#define enable_dc D8
#define direction_dc 5 // Left (0) and right (1)
//end

//Servo motors setup
#define servo1 9
#define servo2 10
#define servo3 11

void setup() {
pinMode(jack,INPUT);

delay(30000); // flight time

switch(1){

  case 1: {

  }

  case 2: {
      //turn on the dc motor
      digitalWrite(enable_dc,HIGH);
      analogWrite()
  }

  case 3: {

  }
}


}

void loop() {
  if(jack == false){

  }

}
