// ==============================
// Pin Definitions
// ==============================
#define enA 10   // Enable1 L298 Pin enA
#define in1 9    // Motor1 L298 Pin in1
#define in2 8    // Motor1 L298 Pin in2
#define in3 7    // Motor2 L298 Pin in3
#define in4 6    // Motor2 L298 Pin in4
#define enB 5    // Enable2 L298 Pin enB

#define ir_R A0  // Right IR/Fire Sensor
#define ir_F A1  // Front IR/Fire Sensor
#define ir_L A2  // Left IR/Fire Sensor

#define servo A4 // Servo Signal Pin
#define pump  A5 // Water Pump Control Pin

int Speed = 160;   // Motor Speed (0-255)
int s1, s2, s3;

// ==============================
// Setup
// ==============================
void setup() {
  Serial.begin(9600);

  pinMode(ir_R, INPUT);
  pinMode(ir_F, INPUT);
  pinMode(ir_L, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(servo, OUTPUT);
  pinMode(pump, OUTPUT);

  // Servo Initial Sweep
  for (int angle = 90; angle <= 140; angle += 5) servoPulse(servo, angle);
  for (int angle = 140; angle >= 40; angle -= 5) servoPulse(servo, angle);
  for (int angle = 40; angle <= 95; angle += 5) servoPulse(servo, angle);

  analogWrite(enA, Speed);
  analogWrite(enB, Speed);

  delay(500);
}

// ==============================
// Loop (Main Logic)
// ==============================
void loop() {

  s1 = analogRead(ir_R);
  s2 = analogRead(ir_F);
  s3 = analogRead(ir_L);

  Serial.print(s1); Serial.print("\t");
  Serial.print(s2); Serial.print("\t");
  Serial.println(s3);

  delay(50);

  // ==========================================
  // Fire Detection — Right Sensor
  // ==========================================
  if (s1 < 250) {
    Stop();
    digitalWrite(pump, HIGH);

    for (int angle = 90; angle >= 40; angle -= 3) servoPulse(servo, angle);
    for (int angle = 40; angle <= 90; angle += 3) servoPulse(servo, angle);
  }

  // ==========================================
  // Fire Detection — Front Sensor
  // ==========================================
  else if (s2 < 350) {
    Stop();
    digitalWrite(pump, HIGH);

    for (int angle = 90; angle <= 140; angle += 3) servoPulse(servo, angle);
    for (int angle = 140; angle >= 40; angle -= 3) servoPulse(servo, angle);
    for (int angle = 40; angle <= 90; angle += 3) servoPulse(servo, angle);
  }

  // ==========================================
  // Fire Detection — Left Sensor
  // ==========================================
  else if (s3 < 250) {
    Stop();
    digitalWrite(pump, HIGH);

    for (int angle = 90; angle <= 140; angle += 3) servoPulse(servo, angle);
    for (int angle = 140; angle >= 90; angle -= 3) servoPulse(servo, angle);
  }

  // ==========================================
  // Obstacle Handling / Navigation
  // ==========================================
  else if (s1 >= 251 && s1 <= 700) {
    digitalWrite(pump, LOW);
    backword();
    delay(100);
    turnRight();
    delay(200);
  }

  else if (s2 >= 251 && s2 <= 800) {
    digitalWrite(pump, LOW);
    forword();
  }

  else if (s3 >= 251 && s3 <= 700) {
    digitalWrite(pump, LOW);
    backword();
    delay(100);
    turnLeft();
    delay(200);
  }

  // Stop Condition
  else {
    digitalWrite(pump, LOW);
    Stop();
  }

  delay(10);
}

// ==============================
// Servo Function
// ==============================
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;  // Convert angle to microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(50);  // Servo refresh
}

// ==============================
// Motor Movements
// ==============================
void forword() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backword() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
