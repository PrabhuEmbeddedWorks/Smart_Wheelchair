// Stores the previous time for millis() based loop execution
unsigned long previousMillis = 0;
unsigned long pt = 0; // Fix: Declare pt to avoid compilation errors

// Stores received Bluetooth data
char BluetoothData;

// Sensor and Motor Pin Assignments
const int trigPin = 8;
const int echoPin = 9;

// Motor control pins
int left_motor_1 = 4;  // Left motor first pin
int left_motor_2 = 5;  // Left motor second pin
int right_motor_1 = 6; // Right motor first pin
int right_motor_2 = 7; // Right motor second pin

// IR sensor pin
int irlpin = 12;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Set pin modes for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set pin modes for motor control
  pinMode(left_motor_1, OUTPUT);
  pinMode(left_motor_2, OUTPUT);
  pinMode(right_motor_1, OUTPUT);
  pinMode(right_motor_2, OUTPUT);

  // Set pin mode for IR sensor
  pinMode(irlpin, INPUT);

  // Set all motor outputs to LOW (stopping the motors)
  digitalWrite(left_motor_1, LOW);
  digitalWrite(left_motor_2, LOW);
  digitalWrite(right_motor_1, LOW);
  digitalWrite(right_motor_2, LOW);
}

void loop() {
  // Check if Bluetooth data is available
  if (Serial.available()) {
    BluetoothData = Serial.read(); // Read Bluetooth input

    // Motor control based on received Bluetooth data
    if (BluetoothData == '1') { // Move forward
      forward();
    }
    if (BluetoothData == '2') { // Move backward
      backward();
    }
    if (BluetoothData == '3') { // Turn left
      left();
    }
    if (BluetoothData == '4') { // Turn right
      right();
    }
    if (BluetoothData == '5') { // Stop
      stopcar();
    }
  }

  // Ultrasonic sensor-based obstacle detection (every 300ms)
  if (millis() - pt > 300) {
    long duration;
    
    // Trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read echo pin to measure distance
    duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2; // Convert to cm

    // Check if an obstacle is within 2-30 cm range
    if (distance < 30 && distance > 2) {
      Serial.println("Obstacle detected. Auto braking");
      stopcar();
      delay(2000); // Pause for 2 seconds
    }

    // IR sensor check for obstacle
    if (!digitalRead(irlpin)) { // Fix: Corrected spelling
      Serial.println("Obstacle detected by IR sensor. Auto braking");
      stopcar();
      delay(2000);
    }

    // Update last checked time
    pt = millis();
  }
}

// Function to move forward
void forward() {
  digitalWrite(left_motor_1, HIGH);
  digitalWrite(left_motor_2, LOW);
  digitalWrite(right_motor_1, LOW);
  digitalWrite(right_motor_2, HIGH);
}

// Function to move backward
void backward() {
  digitalWrite(left_motor_1, LOW);
  digitalWrite(left_motor_2, HIGH);
  digitalWrite(right_motor_1, HIGH);
  digitalWrite(right_motor_2, LOW);
}

// Function to turn left
void left() {
  digitalWrite(left_motor_1, HIGH);
  digitalWrite(left_motor_2, LOW);
  digitalWrite(right_motor_1, HIGH);
  digitalWrite(right_motor_2, LOW);
}

// Function to turn right
void right() {
  digitalWrite(left_motor_1, LOW);
  digitalWrite(left_motor_2, HIGH);
  digitalWrite(right_motor_1, LOW);
  digitalWrite(right_motor_2, HIGH);
}

// Function to stop the car
void stopcar() {
  digitalWrite(left_motor_1, LOW);
  digitalWrite(left_motor_2, LOW);
  digitalWrite(right_motor_1, LOW);
  digitalWrite(right_motor_2, LOW);
}
