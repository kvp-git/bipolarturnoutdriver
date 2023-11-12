// bipolar pulsed coil turnout driver for the Adafruit motor shield v1
// usable for Tomix and Kato bipolar pulsed coil turnouts
// by KVP in 2023 (license: CC BY-NC-SA)

// timing values
#define PULSE_TIME1 (50)   // coil on pulse time
#define PULSE_TIME2 (200)  // coil off pulse time
#define WAIT_TIME1  (100)  // wait time for button release
#define WAIT_TIME2  (1000) // wait time between pulses

// bit positions in the 74HCT595 shift register output
#define MOTOR1_A (2)
#define MOTOR1_B (3)
#define MOTOR2_A (1)
#define MOTOR2_B (4)
#define MOTOR4_A (0)
#define MOTOR4_B (6)
#define MOTOR3_A (5)
#define MOTOR3_B (7)

// arduino pin names for interface to the 74HCT595 latch
#define MOTORLATCH  (12)
#define MOTORCLK    (4)
#define MOTORENABLE (7)
#define MOTORDATA   (8)

// arduino pin names for the enable pins
#define MOTOR1_ENA  (11)
#define MOTOR2_ENA  (3)
#define MOTOR3_ENA  (6)
#define MOTOR4_ENA  (5)

void latchTx(uint8_t latch_state)
{
  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(MOTORDATA, LOW);
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(MOTORCLK, LOW);
    if (latch_state & (1 << (7 - i)))
      digitalWrite(MOTORDATA, HIGH);
    else 
      digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORCLK, HIGH);
  }
  digitalWrite(MOTORLATCH, HIGH);
}

#define MOTOR_NUM (4)

const int outputs[MOTOR_NUM][3] = 
{
  {MOTOR1_A, MOTOR1_B, MOTOR1_ENA},
  {MOTOR2_A, MOTOR2_B, MOTOR2_ENA},
  {MOTOR3_A, MOTOR3_B, MOTOR3_ENA},
  {MOTOR4_A, MOTOR4_B, MOTOR4_ENA},
};

#define INPUT_NUM (6)

const int inputs[INPUT_NUM] =
{
  A0, A1, A2, A3, A4, A5
};

const int routes[INPUT_NUM][MOTOR_NUM] =
{
  { 0, -1, -1, -1},
  { 1,  0, -1, -1},
  { 1,  1,  0, -1},
  { 1,  1,  1,  0},
  { 1,  1,  1,  1},
  {-1, -1, -1, -1}, // unused
};

void pulseOutput(uint8_t motorNum, int dir)
{
  if ((motorNum >= MOTOR_NUM) || (dir < 0) || (dir > 1))
    return;
  Serial.print(" ");
  Serial.print(motorNum);
  Serial.print(":");
  Serial.print(dir);
  latchTx(1 << outputs[motorNum][dir]);
  digitalWrite(outputs[motorNum][2], HIGH);
  delay(PULSE_TIME1);
  digitalWrite(outputs[motorNum][2], LOW);
  delay(PULSE_TIME2);
  latchTx(0);
}

void setup()
{
  Serial.begin(115200);
  pinMode(MOTORLATCH, OUTPUT);
  digitalWrite(MOTORLATCH, LOW);
  pinMode(MOTORENABLE, OUTPUT);
  digitalWrite(MOTORENABLE, LOW);
  pinMode(MOTORDATA, OUTPUT);
  digitalWrite(MOTORDATA, LOW);
  pinMode(MOTORCLK, OUTPUT);
  digitalWrite(MOTORCLK, LOW);
  for (uint8_t i = 0; i < MOTOR_NUM; i++)
  {
    pinMode(outputs[i][2], OUTPUT);
    digitalWrite(outputs[i][2], LOW);
  }
  latchTx(0);
  for (uint8_t i = 0; i < INPUT_NUM; i++)
    pinMode(inputs[i], INPUT_PULLUP);
  delay(1000);
  Serial.println("");
  Serial.println("bipolar turnout driver by KVP");
}

void loop()
{
  for (uint8_t i = 0; i < INPUT_NUM; i++)
  {
    if (digitalRead(inputs[i]) == LOW)
    {
      Serial.print("route:");
      Serial.print(i);
      for (uint8_t n = 0; n < MOTOR_NUM; n++)
        pulseOutput(n, routes[i][n]);
      Serial.println("");
      delay(WAIT_TIME2);
      while (digitalRead(inputs[i]) == LOW)
        delay(WAIT_TIME1);
    }
  }
}

