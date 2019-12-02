  #include <Smartcar.h>
  #include <Encoder.h>
  #include <SoftwareSerial.h> 
  
  Car car;
  Gyroscope gyro;
  Odometer encoderLeft, encoderRight;
  SR04 sonar;
  SR04 sonar2;
  SR04 sonar3;
  Servo myServo;
  Servo myServo2;

  int distanceCounter = 0;
  int motorSpeed = 80; //80% of the max speed
  
  const int TRIGGER_PIN2 = 5; //RIGHT TRIGGER_PIN
  const int ECHO_PIN2 = 4;    //RIGHT ECHO_PIN
  
  const int TRIGGER_PIN = 6; //BACK TRIGGER_PIN
  const int ECHO_PIN = 7;    ////BACK ECHO_PIN
  
  const int TRIGGER_PIN3 = A11; //FRONT TRIGGER_PIN
  const int ECHO_PIN3 = A10; //FRONT ECHO_PIN
  
  const int speed = 100;
  
  const int fSpeed = 70; //70% of the full speed forward
  const int bSpeed = -70; //70% of the full speed backward
  
  const int lDegrees = -75; //degrees to turn left
  const int rDegrees = 90; //degrees to turn right

 int ledRight = A15; //Right LEDS
 int ledLeft = A12; //Left LEDS
int ledBack = A14;//back LEDS
  
  const int encoderLeftPin = 2;
  const int encoderRightPin = 3;
  boolean breaker = true;
  boolean frontObst =true;
  boolean backObst =true;
  boolean stopper = true;
  
  
  //Blinker forward right
  const int blinkerfwr = 0;
  
  //Blinker forward left
  const int blinkerfwl = 0;
  
  //Blinker backward right
  const int blinkerbwr = 0;
  
  //Blinker backward left
  const int blinkerbwl = 0;
  
  //led for backlight when going backwards
  const int ledbw1 = 0;
  
  //led for backlight when going backwards
  const int ledbw2 = 0;
  
  
  int pos = 90;
  
  SoftwareSerial Bluetooth(52, 53); //CHANGE PINS! PIN 13 DOES NOT WORK
  
  
  void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
  gyro.attach();
  myServo.attach(A0);
  myServo.write(90);
  myServo2.attach(A1);
  myServo2.write(0);

  pinMode(ledRight, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledBack, OUTPUT);
  
  encoderLeft.attach(encoderLeftPin);
  encoderRight.attach(encoderRightPin);
  sonar.attach(TRIGGER_PIN2 , ECHO_PIN2); //RIGHT
  sonar2.attach(TRIGGER_PIN, ECHO_PIN); //REAR
  sonar3.attach(TRIGGER_PIN3, ECHO_PIN3); //FRONT
  gyro.begin(7);
  car.begin(encoderLeft, encoderRight, gyro);
  car.updateMotors();
  myServo2.write(90);
  }
  
  
  
  
  void loop() {

//starts handling input from the controller.

handleInput();

gyro.update();

  
  }
  
  void checker(){
    if(car.getSpeed() > sonar3.getDistance() && sonar3.getDistance() != 0){
      if(car.getSpeed() <= 20 && sonar3.getDistance() < 20 && sonar3.getDistance() != 0){
        car.setSpeed(0);
      }
  
        else{
        car.setSpeed(car.getSpeed() - 4);
        }
    }
  
    return;
  }
  
  
  
  
  void angleCorrection(){
    //checks if the car is supposed to go straight ahead
    if(car.getAngle() == 0  && car.getSpeed() > 0){
      //If the gyroscopre detects that the car is going left or right is caught in this if statement
      if(gyro.getAngularDisplacement() >= 4 && gyro.getAngularDisplacement() < 160){
        Serial.println("Correction made");
        // when it's not supposed to, it corrects the car by lowering the motorspeed on one side
        car.setMotorSpeed(fSpeed/1.6,fSpeed);
        delay(200);
        //Makes is same speed again
        car.setMotorSpeed(fSpeed, fSpeed);
      }
  
      if(gyro.getAngularDisplacement() >= 200 && gyro.getAngularDisplacement() <= 356){
        Serial.println("Correction made");
        car.setMotorSpeed(fSpeed, fSpeed/1.6);
        delay(200);
        car.setMotorSpeed(fSpeed, fSpeed);
      }
  
      //Starts the loop again
  
      return;
    }
    else{
      //if the car is supposed to turn, the just calling the loop again.
      return;
  
    }
  }
  //forward and backward for voice control
  void goForward(){
  frontObst=false;
  while(sonar3.getDistance() > 30 || sonar3.getDistance() == 0){
  Serial.println("no obstacles found ");
  Serial.println(sonar3.getDistance());
  car.setSpeed(50);
  car.setAngle(0);
  delay(200);
  }
  
  if(sonar3.getDistance() < 31 && sonar3.getDistance() != 0){
  Serial.println("obstacle found");
  car.setSpeed(0);
  car.setAngle(0);
  delay(200);
  return;
   }
  return;
  }
  
  
  
  void goBackward(){
  myServo.write(40);
  backObst=false;
  while(sonar2.getDistance() > 30 || sonar2.getDistance() == 0){
  Serial.println("no obstacles found ");
  Serial.println(sonar2.getDistance());
  car.setSpeed(-50);
  car.setAngle(0);
  delay(200);
  }
  
  if(sonar2.getDistance() < 31 && sonar2.getDistance() != 0){
  Serial.println("obstacle found");
  car.setSpeed(0);
  car.setAngle(0);
  delay(200);
  return;
  }
  return;
  
  }
  
  
  
  void handleInput() { //handle serial input if there is any
      char input;
      while (Serial2.available() || Serial.available()) {
        input = 'y';
        if(Serial2.available()){
          input = Serial2.read(); //read until the last character
        }
        if(Serial.available()){
          input = Serial.read(); //read until the last character
        }
        switch (input) {
  
        case 'l': //rotate counter-clockwise going forward
        Serial.println("Left");
        car.setSpeed(fSpeed); //30% of the full speed
        car.setAngle(-75); //75 degrees to the left
        break;
  
        case 'r': //turn clock-wise
        Serial.println("Right");
        car.setSpeed(fSpeed);
        car.setAngle(75); //75 to the right
        break;
  
        case 'f': //go ahead
        Serial.println("Drive Forward");
        digitalWrite(ledBack, LOW);
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
  
        case 'b': //go back
        digitalWrite(ledBack, HIGH);
        Serial.println("Drive backward");
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
  
        case 'g':  // Forward Left
        Serial.println("Forward Left");
        car.setMotorSpeed(speed/3, speed);
        break;
  
        case 'i':  // Forward Right
        Serial.println("Forward Right");
        car.setMotorSpeed(speed, speed/3);
        break;
  
        case 'h':  // Back Left
        Serial.println("Back Left");
        digitalWrite(ledBack, HIGH);
        car.setMotorSpeed(0, -speed);
        digitalWrite(ledBack, LOW);
        break;
  
        case 'j':  // Back Right
        Serial.println("Back Right");
        digitalWrite(ledBack, HIGH);
        car.setMotorSpeed(-speed, 0);
        digitalWrite(ledBack, LOW);
        break;
  
        case 'z':
        if(pos >= 20 || pos <= 180){
          int test = pos;
          for(int i = test; i > test-20; i--){
            Serial.print("h");
            pos = i;
            delay(10);
            myServo2.write(pos);
          }
        }
        break;
  
        case 'x':
        if(pos >= 0 || pos <= 160){
          int test = pos;
          for(int i = test; i < test + 20; i++){
            Serial.print("j");
            pos = i;
            delay(10);
            myServo2.write(pos);
          }
        }
        break;
  
  
        case 'o': //go ahead
        Serial.println("Drive Forward");
        while(frontObst){
        goForward();
        }
        frontObst=true;
        break;
  
        case 'k': //go back
        Serial.println("Drive backward");
        digitalWrite(ledBack, HIGH);
        while(backObst){
        goBackward();
        }
        backObst=true;
        digitalWrite(ledBack, LOW);
        break;
  
        case 'p': //start automatic parking
        Serial.println("Parking");
        digitalWrite(ledRight, LOW);
        while (breaker){
        findSpot();
        }
  
        Serial.println("returned to parking");
        breaker = true;
        return;
        break;

        case 'e': //exiting function
        Serial.println("Exiting spot");{
        exitSpot();
         } 
        break;
  
        default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
        car.setMotorSpeed(0, 0);
        return;
        }
      }
      return;
    }
  
  
  
  
  
    void findSpot(){
    myServo.write(10);

    car.setMotorSpeed(40,40);

    if(sonar.getDistance() == 0 || sonar.getDistance() > 20 ){
    encoderLeft.begin();
    encoderRight.begin();
    Serial.println("Encoder activated");
    //car.setSpeed(30);
  
    while(sonar.getDistance() == 0 || sonar.getDistance() > 20){
    Serial.println("Space found, look for more");
    if(encoderLeft.getDistance() > 30) {
      Serial.println("Free spot detected, start parking!");

      car.setMotorSpeed(40,40);

      car.setSpeed(0);
      car.setAngle(0);
      delay(2000);
      AutoPark();
      Serial.println("returned to findspot");
      return;
      }
    }
    // car.setSpeed(35);

    car.setMotorSpeed(40 ,40);

    Serial.println("No space found, keep driving straight");
    }
    else{
  
    }
  }
  
  
  
  
  //parking function
  void AutoPark(){
    digitalWrite(ledBack, HIGH);
    blinkersRight();
  Serial.println("backward Right");
  encoderLeft.begin();
  encoderRight.begin();
  

  while(encoderRight.getDistance() < 11){
    Serial.println("encoder values");
    Serial.println(encoderRight.getDistance());
    gyro.update();
    rotateOnSpot(-2);
    delay(400);
   // blinkersRight();
  }
  stopper = true;
  distanceCounter =0;

  
  do{
    distanceCounter = distanceCounter +1;
    Serial.println("Servo hit");
    servoTurning();
    myServo.write(1);
    delay(300);
    Serial.println("finish-statement hit");
    for(pos = 1; pos <= 110; pos += 2){
      myServo.write(pos);
      delay(20);

      if(sonar2.getDistance() < 6 && sonar2.getDistance() != 0){
        pos = pos +1;
        myServo.write(pos);
        if(sonar2.getDistance() < 6 && sonar2.getDistance() != 0){
          Serial.println(sonar2.getDistance());

         stopper = false;
        }
      }
    }

   // car.go(-1);

    blinkersRight();
  }
  
  while(stopper && distanceCounter < 8);
  
  car.go(1);
 digitalWrite(ledBack, LOW);
  Serial.println("Forward Right");
  gyro.update();
  encoderLeft.begin();
  encoderRight.begin();
  while(encoderLeft.getDistance() < 9){
    gyro.update();
    rotateOnSpot(2);
    delay(400);
  }
  
  delay(150);
  car.setSpeed(0);
  Serial.println("AutoPark: DONE.");
  breaker = false;
  Serial.println("braker = false, returns to autopark");
  finalCorrection();
  return;

  }
  
  //somewhat working for final correction of the car when parking.
  void finalCorrection(){
  //sonar2 = rear
  //sonar3 = front
  
  //the servo should make sonar2 look backwards. 45 is the perfect angle.
  myServo.write(45);
  while (true){
  
  //Checking distance back and front, and saving them
  int back = sonar2.getDistance();
  int front = sonar3.getDistance();
  
  // If the distance is = 0 in the back or in the front, we know that we can move a bit in that direction.
  if(back == 0){
  if(front != 0 && front > 12){
  car.go(2);
  }
  }

  
  else if(front == 0){
  if(back > 0 && back > 12){
  car.go(-2);
  }
  }
  
  else{
  
  //If the distance in the back and in the front is the same (with a margin of +- 2), then we break
  if(back == front || back+1 == front || back-1 == front || back-2 == front || back+2 == front){
  break;
  
  }
  //if there is a lot of distance in both front and back (0 = longer tham the sensors can read), then we break
  if(back == 0 && front == 0 || back > 50 && front > 50 ){
  break;
  }
  
  else{
  //Basically, this is the "standard" case, where we get 2 values, where one is a bit bigger than the other,
  // and we correct the car by going in one direction.
  if(back > front){
  car.go(-2);
  delay(200);
  }
  
  if(front > back){
  car.go(2);
  delay(200);
  }
  
  }
  }
  }
  return;
  }




void exitSpot(){
 //back
  encoderLeft.begin();
  encoderRight.begin();
  blinkersLeft();
  delay(100);
 while(encoderRight.getDistance() < 9){
    Serial.println("encoder values");
    Serial.println(encoderRight.getDistance());
    gyro.update();
    rotateOnSpot(-5);
    delay(750);
  }
  car.setMotorSpeed(40,40);
  delay(700);
  car.setMotorSpeed(0, 0);
  delay(100);
  encoderLeft.begin();
  encoderRight.begin();
  delay(100);
  while(encoderRight.getDistance() < 9){
    Serial.println("encoder values");
    Serial.println(encoderRight.getDistance());
    gyro.update();
    rotateOnSpot(5);
    delay(750);
   
   // blinkersRight();
  }
  car.setMotorSpeed(40,40);
  delay(300);
  car.setMotorSpeed(0, 0);
}
  
  void servoTurning(){
  myServo.write(1);
  delay(300);
  // goes from 1 degree to 110 degrees
  for (pos = 1; pos <= 110; pos += 2) {

    Serial.println("sonar values");

  Serial.println(sonar2.getDistance());
  // tell servo to go to position in variable 'pos'
  myServo.write(pos);
  // waits 50ms for the servo to reach the position
  delay(20);
  
  // If the sonar finds a value that is lesser than 7, and ofc not 0
  if(sonar2.getDistance() < 9 && sonar2.getDistance() != 0){
  delay(20);
  //Does the same check again, in order to prevent faulty values
  if(sonar2.getDistance() < 9 && sonar2.getDistance() != 0){
  Serial.println("back from sevorurning");
  return;
  }
  }

  }
  //if we have not returned, then we know can we can "go" 5 distances back.
  car.go(-2);
  
  
  //We repeat this, but this time the sonar starts from the other direction, and goes back.
  // goes from 110 degrees to 1 degree
  for (pos = 110; pos <= 1; pos -= 2) {
  Serial.println(sonar2.getDistance());
  // tell servo to go to position in variable 'pos'
  myServo.write(pos);
  // waits 50ms for the servo to reach the position
  delay(50);
  //same as above.
  if(sonar2.getDistance() < 7 && sonar2.getDistance() != 0){
  delay(50);
  Serial.println(sonar2.getDistance());
  if(sonar2.getDistance() < 7 && sonar2.getDistance() != 0){
  Serial.println("back from sevorurning");
  return;
  }
  }
  }
  car.go(-2);
  
  }


  void blinkersRight(){ //This cause the blink effect on the right side blinkers 
    digitalWrite(ledRight, HIGH); 
    delay(40);
    digitalWrite(ledRight, LOW);
    delay(20);   
  }

  void blinkersLeft(){ //This cause the blink effect on the left side blinkers
    digitalWrite(ledLeft, HIGH); 
    delay(40);
    digitalWrite(ledLeft, LOW);
    delay(20);   
  }

 void rotateOnSpot(int targetDegrees) {
  targetDegrees %= 360; //put it on a (-360,360) scale
  if (!targetDegrees) return; //if the target degrees is 0, don't bother doing anything
  /* Let's set opposite speed on each side of the car, so it rotates on spot */
  if (targetDegrees > 0) { //positive value means we should rotate clockwise
    car.setMotorSpeed(motorSpeed, -motorSpeed); // left motors spin forward, right motors spin backward
  } else { //rotate counter clockwise
    car.setMotorSpeed(-motorSpeed, motorSpeed); // left motors spin backward, right motors spin forward
  }
  unsigned int initialHeading = gyro.getAngularDisplacement(); //the initial heading we'll use as offset to calculate the absolute displacement
  int degreesTurnedSoFar = 0; //this variable will hold the absolute displacement from the beginning of the rotation
  while (abs(degreesTurnedSoFar) < abs(targetDegrees)) { //while absolute displacement hasn't reached the (absolute) target, keep turning
    gyro.update(); //update to integrate the latest heading sensor readings
    int currentHeading = gyro.getAngularDisplacement(); //in the scale of 0 to 360
    if ((targetDegrees < 0) && (currentHeading > initialHeading)) { //if we are turning left and the current heading is larger than the
      //initial one (e.g. started at 10 degrees and now we are at 350), we need to substract 360, so to eventually get a signed
      currentHeading -= 360; //displacement from the initial heading (-20)
    } else if ((targetDegrees > 0) && (currentHeading < initialHeading)) { //if we are turning right and the heading is smaller than the
      //initial one (e.g. started at 350 degrees and now we are at 20), so to get a signed displacement (+30)
      currentHeading += 360;
    }
    degreesTurnedSoFar = initialHeading - currentHeading; //degrees turned so far is initial heading minus current (initial heading
    //is at least 0 and at most 360. To handle the "edge" cases we substracted or added 360 to currentHeading)
  }
  car.stop(); //we have reached the target, so stop the car
}
