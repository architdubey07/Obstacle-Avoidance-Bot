// ===============================
//      OBSTACLE AVOIDING ROBOT
// Arduino Nano/Uno + HC-SR04
// L298N + 2 DC Motors
// ===============================

// ---------- Ultrasonic ----------
#define trig 11
#define echo 12

// ---------- L298N Motor Driver ----------
#define ENA 5
#define IN1 7
#define IN2 8

#define IN3 9
#define IN4 10
#define ENB 6

// ---------- Settings ----------
#define OBSTACLE_DISTANCE 15
#define MOTOR_SPEED 150
#define TURN_SPEED 160

long duration;
int distance;

bool turnRightNext = true;


// ======================================
//              SETUP
// ======================================

void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();

  Serial.begin(9600);

  delay(1000);
}


// ======================================
//          GET DISTANCE
// ======================================

int getDistance() {

  // Make sure trigger is LOW
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Send 10 microsecond pulse
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read echo
  duration = pulseIn(echo, HIGH, 30000);

  // If no echo received
  if (duration == 0) {
    return 400;
  }

  // Speed of sound = 0.034 cm/us
  int d = duration * 0.034 / 2;

  return d;
}


// ======================================
//              FORWARD
// ======================================

void forward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
}


// ======================================
//              BACKWARD
// ======================================

void backward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
}


// ======================================
//                STOP
// ======================================

void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


// ======================================
//             TURN RIGHT
// ======================================

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);

  delay(500);

  stopMotors();
}


// ======================================
//              TURN LEFT
// ======================================

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);

  delay(500);

  stopMotors();
}


// ======================================
//                LOOP
// ======================================

void loop() {

  distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // ----------------------------------
  // NO OBSTACLE
  // ----------------------------------

  if (distance > OBSTACLE_DISTANCE) {

    // Slow down slightly when approaching
    if (distance < 25) {

      analogWrite(ENA, 110);
      analogWrite(ENB, 110);

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

    }
    else {

      forward();
    }
  }


  // ----------------------------------
  // OBSTACLE DETECTED
  // ----------------------------------

  else {

    // Stop immediately
    stopMotors();
    delay(200);


    // Move backward slightly
    backward();
    delay(250);

    stopMotors();
    delay(200);


    // --------------------------------
    // Alternate turning direction
    // --------------------------------

    if (turnRightNext) {

      turnRight();

      turnRightNext = false;
    }

    else {

      turnLeft();

      turnRightNext = true;
    }

    delay(200);
  }

  delay(50);
}
