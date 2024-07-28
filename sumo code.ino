// Initial value of the variable:
// ML2 Edge Sensor (DIGITAL PIN)
const int BACK_RIGHT_line_SENSOR_PIN = A5; // or A7
const int FRONT_LEFT_line_SENSOR_PIN = A1;
const int BACK_LEFT_line_SENSOR_PIN = A3;
const int FRONT_RIGHT_line_SENSOR_PIN = A7; // or A5
const int UNDER_CENTER_line_SENSOR_PIN = A2; // or A5


const int LPWM = 5; // between one of those 5/6/9/10 *preferred 5 or 6*
const int RPWM = 6; // between one of those 5/6/9/10 *preferred 5 or 6*
const int LEN = 7;
const int REN = 8;

// JS200XF Opponent Sensor
const int FRONT_LEFT_js_SENSOR_PIN = 14;
const int FRONT_RIGHT_js_SENSOR_PIN = 20;

// mz sensor
const int FRONT_center_Mz_SENSOR_PIN = 18;
const int BACK_center_Mz_SENSOR_PIN = 38;
const int LEFT_center_Mz_SENSOR_PIN = 12;
const int RIGHT_center_Mz_SENSOR_PIN = 22;

const int WIGHT_VALUE = 500 ; 
const int BLACK_VALUE = 650 ; 


void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  }
}
void MOVE_FORWARD(int right, int left) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, right); // Adjust speed as needed
  analogWrite(LPWM, left); // Adjust speed as needed
}

void MOVE_BACKWARD(int right, int left) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, right); // Adjust speed as needed
  analogWrite(LPWM, left); // Adjust speed as needed
}

void MOVE_STOP() {
  digitalWrite(REN, LOW);
  digitalWrite(LEN, LOW);
  analogWrite(RPWM, 128); // Stop
  analogWrite(LPWM, 128); // Stop
}

void MOVE_ANGLE_RIGHT(int slightRightSpeed, int fullFwdSpeed) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, slightRightSpeed); // Slight Right Turn Speed
  analogWrite(LPWM, fullFwdSpeed); // Full FWD Speed
}

void MOVE_SHARP_RIGHT(int sharpRightSpeed, int fullFwdSpeed) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, sharpRightSpeed); // Sharp Right Turn Speed
  analogWrite(LPWM, fullFwdSpeed); // Full FWD Speed
}

void MOVE_ANGLE_LEFT(int fullFwdSpeed, int slightLeftSpeed) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, fullFwdSpeed); // Full FWD Speed
  analogWrite(LPWM, slightLeftSpeed); // Slight Left Turn Speed
}

void MOVE_SHARP_LEFT(int fullFwdSpeed, int sharpLeftSpeed) {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, fullFwdSpeed); // Full FWD Speed
  analogWrite(LPWM, sharpLeftSpeed); // Sharp Left Turn Speed
}


void MOVE_SHARP_LEFT() {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, 252); // Full FWD Speed
  analogWrite(LPWM, 3); // Sharp Left Turn Speed
}

void brake() {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, 127); // Stop
  analogWrite(LPWM, 127); // Stop
}

void front_strategy() {
  bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  unsigned long startTime = millis();

  if (frontMz) {
    // Continue reading the sensor for 5 seconds
    while (digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH) {
      if (millis() - startTime >= 5000) {
        // If the opponent is detected for 5 seconds, execute a sharp right turn
        MOVE_SHARP_RIGHT(3, 252);
        delay(1250); // Adjust delay as needed for appropriate turn duration
        brake(); // Stop the robot after the turn
        return;
      }
    }
  }
}


void line_control() {
  bool backSensor = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH; // Adjust pin as needed for your distance sensor
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;


  if (digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH && digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == LOW) {
      MOVE_BACKWARD(3, 3);
      delay(2500);
      brake();
      MOVE_SHARP_RIGHT(3 , 252);
      delay(2500);
      brake();
  } else if (digitalRead(FRONT_LEFT_line_SENSOR_PIN) == LOW && digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH) {
      MOVE_BACKWARD(3 , 3);
      delay(2500);
      brake();
      MOVE_SHARP_LEFT(252 , 3);
      delay(2500);
      brake();
  } else if (digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH && digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH) {
      MOVE_BACKWARD(3 , 3);
      delay(2500);
      brake();
      MOVE_SHARP_RIGHT(3,252);
      delay(2500);
      brake();
  } else if (digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH || digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH) {
      MOVE_FORWARD(200 , 200);
      delay(1500);
      brake();
    }/*else if (digitalRead(UNDER_CENTER_line_SENSOR_PIN) == HIGH) {
    MOVE_BACKWARD(3, 3);
    delay(2500);
    brake(); }*/
    else if (backSensor) {
      MOVE_SHARP_RIGHT(252, 3);
      delay(2500);
      brake();
    }
  
  else if (digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH || digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH || digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH)  {
    MOVE_FORWARD(252 , 252);
    delay(2500);
    }
    else {
MOVE_FORWARD(200, 200);  }
}

/*
void attack_strategy() {
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;
  bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  bool backMz = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH;
  bool leftMz = digitalRead(LEFT_center_Mz_SENSOR_PIN) == HIGH;
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;
  bool frontLeft = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRight = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  bool backLeft = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool backRight = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;

  if (frontMz) {
    MOVE_FORWARD();
  } else if (backMz) {
    MOVE_BACKWARD();
  } else if (leftMz) {
    MOVE_SHARP_LEFT();
  } else if (rightMz) {
    MOVE_SHARP_RIGHT();
  } else if (frontLeftJS || leftMz) {
    MOVE_ANGLE_LEFT();
  } else if (frontRightJS || rightMz) {
    MOVE_ANGLE_RIGHT();
  } else {
    MOVE_FORWARD();
  }
}

void majnon() {
  MOVE_SHARP_RIGHT();
}

void henka() {
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;
  if (rightMz) {
    MOVE_BACKWARD();
    delay(750);
    MOVE_STOP();
    delay(1500);
  }
}

void strat1() {
  majnon();
  henka();
}

void stop() {
  if (FRONT_RIGHT_line_SENSOR_PIN == 1 || BACK_LEFT_line_SENSOR_PIN == 1 || BACK_RIGHT_line_SENSOR_PIN == 1) {
    MOVE_STOP();
  }
}

void mo_angle_attack() {
  int mz = digitalRead(RIGHT_center_Mz_SENSOR_PIN);
  int ljs = digitalRead(FRONT_LEFT_js_SENSOR_PIN);
  int rjs = digitalRead(FRONT_RIGHT_js_SENSOR_PIN);
  
  stop();

  if (mz == 1) {
    MOVE_BACKWARD();
    delay(800);
    while (ljs == 0 || rjs == 0) {
      MOVE_STOP();
    }
    if (ljs == 1 || rjs == 1) {
      MOVE_FORWARD();
      delay(2000);
    }
  }
}

void angle_attack() {
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;
  bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  bool backMz = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH;
  bool leftMz = digitalRead(LEFT_center_Mz_SENSOR_PIN) == HIGH;
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;

  bool frontLeft = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRight = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  bool backLeft = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool backRight = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;

  while (frontLeft == LOW || frontRight == LOW) {
    if (frontLeftJS || frontRightJS || frontMz) {
      MOVE_ANGLE_LEFT();
      delay(500);
    }
  }
}*/
 void MOVE_ZIGZAG() {
  MOVE_SHARP_RIGHT(3, 252);
  delay(1250);
  MOVE_FORWARD(200, 200);
  delay(3000);
  MOVE_SHARP_LEFT(252 , 3);
  delay(1250);
  MOVE_FORWARD(200, 200);
  delay(3000);
  

}
void circular_search_strategy() {
  bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  bool leftMz = digitalRead(LEFT_center_Mz_SENSOR_PIN) == HIGH;
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;
  

  if (frontMz || leftMz || rightMz) {
    // Opponent detected, engage
    MOVE_FORWARD(200, 200);
  } else {
    // No opponent detected, move in a circle
    MOVE_CIRCLE();
  }
}

void MOVE_CIRCLE() {
  digitalWrite(REN, HIGH);
  digitalWrite(LEN, HIGH);
  analogWrite(RPWM, 150); // Outer wheel speed
  analogWrite(LPWM, 100); // Inner wheel speed
}

void side_attack_strategy() {
  //bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  bool leftMz = digitalRead(LEFT_center_Mz_SENSOR_PIN) == LOW;
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;
  bool backRightLine = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;
  bool frontLeftLine = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool backLeftLine = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRightLine = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  bool rightjs = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;
  bool leftjs = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;


  

  if (backRightLine || frontLeftLine || backLeftLine || frontRightLine) {
    line_control();
  }
 
    else if (leftjs || rightjs) {
    // Opponent directly ahead, move forward
    MOVE_FORWARD(154, 154);
  } else if (leftMz) {
    // Opponent on the left, execute a sharp right turn to flank
    MOVE_SHARP_RIGHT(50, 205);
    delay(1250); // Adjust delay for appropriate turn
    MOVE_FORWARD(154, 154);
  } else if (rightMz) {
    // Opponent on the right, execute a sharp left turn to flank
    MOVE_SHARP_LEFT(205, 50);
    delay(1250); // Adjust delay for appropriate turn
    MOVE_FORWARD(154, 154);
  } else {
    // No opponent detected, move forward or execute zigzag pattern
    MOVE_CIRCLE();
  }
}

void attack_strategy() {
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;
  bool frontMz = digitalRead(FRONT_center_Mz_SENSOR_PIN) == HIGH;
  bool backMz = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH;
  bool leftMz = digitalRead(LEFT_center_Mz_SENSOR_PIN) == HIGH;
  bool rightMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;

  if (rightMz) {
    // Right opponent sensor triggered: turn left and attack
    MOVE_SHARP_LEFT(252, 3);
    delay(1250); // Adjust delay for appropriate turn
    MOVE_FORWARD(200, 200);
  } else if (leftMz) {
    // Left opponent sensor triggered: turn right and attack
    MOVE_SHARP_RIGHT(3, 252);
    delay(1250); // Adjust delay for appropriate turn
    MOVE_FORWARD(200, 200);
  } else if (backMz) {
    // Back opponent sensor triggered: turn around and attack
    MOVE_BACKWARD(200, 200);
    delay(1000); // Adjust delay for appropriate backward movement
    MOVE_SHARP_RIGHT(3, 252);
    delay(2500); // Adjust delay for complete turn around
    MOVE_FORWARD(200, 200);
  } else if (frontMz) {
    // Front opponent sensor triggered: move forward and attack
    MOVE_FORWARD(200, 200);
  }
}
void reactive_attack() {
  bool lefttMz = digitalRead(RIGHT_center_Mz_SENSOR_PIN) == HIGH;
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;

  bool backRightLine = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;
  bool frontLeftLine = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool backLeftLine = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRightLine = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  
  if (backRightLine || frontLeftLine || backLeftLine || frontRightLine) {
    line_control();
  }
  else if (lefttMz) {
    // Move backward at maximum speed when the right MZ sensor detects an opponent
    MOVE_BACKWARD(50, 50);
    delay(1000); // Adjust delay as needed for sufficient backward movement
    brake(); // Stop the robot
  }

    // Check if either JS sensor detects an opponent
    else if (frontLeftJS || frontRightJS) {
      // Move forward at maximum speed to attack
      MOVE_FORWARD(195, 195);
      delay(2000); // Adjust delay as needed for sufficient forward attack
      brake(); // Stop the robot
    }
    else {
      line_control();
    }
  }


  void custom_movement_pattern() {
  bool backRightLine = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;
  bool frontLeftLine = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool backLeftLine = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRightLine = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  
  if (backRightLine || frontLeftLine || backLeftLine || frontRightLine) {
    line_control();
  }
  // Initial forward movement
  MOVE_FORWARD(200, 200);
  delay(2000); // Adjust time for the length of the first straight segment
  
  // First turn (sharp right)
  MOVE_SHARP_RIGHT(100, 252); // Adjust speed for a sharp right turn
  delay(1000); // Adjust time for the turn duration

  // Second forward movement
  MOVE_FORWARD(200, 200);
  delay(2000); // Adjust time for the length of the second straight segment

  // Second turn (sharp left)
  MOVE_SHARP_LEFT(252, 100); // Adjust speed for a sharp left turn
  delay(1000); // Adjust time for the turn duration

  // Third forward movement
  MOVE_FORWARD(200, 200);
  delay(2000); // Adjust time for the length of the third straight segment
  /*
  // Third turn (sharp right)
  MOVE_SHARP_RIGHT(100, 252); // Adjust speed for a sharp right turn
  delay(1000); // Adjust time for the turn duration
  
  // Final forward movement
  MOVE_FORWARD(200, 200);
  delay(2000); // Adjust time for the length of the final straight segment*/
}

void evade_and_attack() {
  bool backSensor = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH; // Adjust pin as needed for your distance sensor
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;

  bool backRightLine = digitalRead(BACK_RIGHT_line_SENSOR_PIN) == HIGH;
  bool frontLeftLine = digitalRead(FRONT_LEFT_line_SENSOR_PIN) == HIGH;
  bool backLeftLine = digitalRead(BACK_LEFT_line_SENSOR_PIN) == HIGH;
  bool frontRightLine = digitalRead(FRONT_RIGHT_line_SENSOR_PIN) == HIGH;
  
  if (backRightLine || frontLeftLine || backLeftLine || frontRightLine) {
    line_control();
  }

  else if (backSensor) {
    // Evade by moving in a curved path
    MOVE_SHARP_LEFT(150, 75); // Sharp left turn with slower speed
    delay(4000); // Adjust delay for the curved path duration

    // Prepare to turn around and attack
    brake(); // Stop briefly before turning
    delay(500); // Short pause

    // Turn towards the opponent
    MOVE_SHARP_RIGHT(75, 150); // Sharp right turn with slower speed
    delay(1500); // Adjust delay for the turn duration

    // Check if either JS sensor detects an opponent
    }else if (frontLeftJS || frontRightJS) {
      // Move forward to attack with slower speed
      MOVE_FORWARD(150, 150);
      delay(3000); // Adjust delay for sufficient forward attack
      brake(); // Stop the robot
    }
   else {
    // No opponent detected, move forward normally at a slower speed
    MOVE_FORWARD(150, 150);
  }
}


void turn_around_and_attack() {
  bool backSensor = digitalRead(BACK_center_Mz_SENSOR_PIN) == HIGH; // Adjust pin as needed for your distance sensor
  bool frontLeftJS = digitalRead(FRONT_LEFT_js_SENSOR_PIN) == HIGH;
  bool frontRightJS = digitalRead(FRONT_RIGHT_js_SENSOR_PIN) == HIGH;

  if (backSensor) {
    // Turn 180 degrees faster
    MOVE_SHARP_RIGHT(150, 252); // Adjust speed for a sharp right turn
    delay(1800); // Adjust delay for the turn duration to complete 180 degrees

    // Check if either JS sensor detects an opponent
    if (frontLeftJS || frontRightJS) {
      // Move forward to attack
      MOVE_FORWARD(252, 252); // Full speed ahead
      delay(2500); // Adjust delay for sufficient forward attack
      brake(); // Stop the robot
    }
  } else {
    // No opponent detected, move forward normally
    MOVE_FORWARD(200, 200);
  }
}


void setup() {
  Serial.begin(9600);

  pinMode(LEN, OUTPUT);
  pinMode(REN, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);

  digitalWrite(LEN, LOW);
  digitalWrite(REN, LOW);

  setPwmFrequency(LPWM, 8);
  setPwmFrequency(RPWM, 8);

  pinMode(BACK_RIGHT_line_SENSOR_PIN, INPUT);
  pinMode(BACK_LEFT_line_SENSOR_PIN, INPUT);
  pinMode(FRONT_RIGHT_line_SENSOR_PIN, INPUT);
  pinMode(FRONT_LEFT_line_SENSOR_PIN, INPUT);

  pinMode(FRONT_LEFT_js_SENSOR_PIN, INPUT);
  pinMode(FRONT_RIGHT_js_SENSOR_PIN, INPUT);

  pinMode(RIGHT_center_Mz_SENSOR_PIN, INPUT);
  pinMode(LEFT_center_Mz_SENSOR_PIN, INPUT);
  pinMode(BACK_center_Mz_SENSOR_PIN, INPUT);
  pinMode(FRONT_center_Mz_SENSOR_PIN, INPUT);
  
  delay(35592);

  /* 180 degree
  MOVE_SHARP_RIGHT(252, 3);
  delay(2500);
  brake();*/

  /*  to left 90 degree
  MOVE_SHARP_RIGHT(252, 100);
  delay(2500);
  brake();*/
  
  /* 45 degree
  MOVE_SHARP_RIGHT(252, 50);
  delay(2500);
  brake();*/
  
 MOVE_FORWARD(252, 252 ) ; 
  
}

void loop() {
line_control();
}