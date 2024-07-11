#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Definición de pines para ESP32-WROOM-32D
#define ir_enter 13
#define ir_back 12

#define ir_car1 14
#define ir_car2 27
#define ir_car3 26
#define ir_car4 25
#define ir_car5 33
#define ir_car6 32
#define SERVO_IN_PIN 18
#define SERVO_OUT_PIN 19

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo_in;
Servo myservo_out;

int S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0, S6 = 0;
int flag1 = 0, flag2 = 0;
int slot = 6;
const int MAX_SLOTS = 6;

void setup() {
  Serial.begin(115200);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_car5, INPUT);
  pinMode(ir_car6, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  // Configuración de los servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  myservo_in.setPeriodHertz(50);
  myservo_out.setPeriodHertz(50);
  myservo_in.attach(SERVO_IN_PIN, 500, 2400);
  myservo_out.attach(SERVO_OUT_PIN, 500, 2400);
  myservo_in.write(90);
  myservo_out.write(90);

  // Inicialización del LCD
  Wire.begin(2, 0);  // SDA, SCL
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("   Easy Parking    ");
  lcd.setCursor(0, 2);
  lcd.print("    Welcome!!!    ");
  delay(3000);
  lcd.clear();

  Read_Sensor();

  int total = S1 + S2 + S3 + S4 + S5 + S6;
  slot = slot - total;
}

void loop() {
  Read_Sensor();

  lcd.setCursor(0, 0);
  lcd.print("Slots: ");
  lcd.print(slot);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print(S1 == 1 ? "S1:Fill " : "S1:Empty");
  lcd.setCursor(10, 1);
  lcd.print(S2 == 1 ? "S2:Fill " : "S2:Empty");

  lcd.setCursor(0, 2);
  lcd.print(S3 == 1 ? "S3:Fill " : "S3:Empty");
  lcd.setCursor(10, 2);
  lcd.print(S4 == 1 ? "S4:Fill " : "S4:Empty");

  lcd.setCursor(0, 3);
  lcd.print(S5 == 1 ? "S5:Fill " : "S5:Empty");
  lcd.setCursor(10, 3);
  lcd.print(S6 == 1 ? "S6:Fill " : "S6:Empty");

  if (digitalRead(ir_enter) == LOW && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        if (S1 == 0 || S2 == 0 || S3 == 0 || S4 == 0 || S5 == 0 || S6 == 0) {
          myservo_in.write(130);
          slot = slot - 1;
        }
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorry, Parking Full");
      delay(2000);
      lcd.clear();
    }
  }

  if (digitalRead(ir_back) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo_out.write(60);
      delay(2000);
      myservo_out.write(90);
      flag2 = 0;
    }
    if (slot < MAX_SLOTS) {
      if (S1 == 0 || S2 == 0 || S3 == 0 || S4 == 0 || S5 == 0 || S6 == 0) {
        slot++;
      }
    }
  }

  if (flag1 == 1 || flag2 == 1) {
    delay(2000);
    myservo_in.write(90);
    myservo_out.write(90);
    flag1 = 0;
    flag2 = 0;
  }
}

void Read_Sensor() {
  S1 = digitalRead(ir_car1) == LOW ? 1 : 0;
  S2 = digitalRead(ir_car2) == LOW ? 1 : 0;
  S3 = digitalRead(ir_car3) == LOW ? 1 : 0;
  S4 = digitalRead(ir_car4) == LOW ? 1 : 0;
  S5 = digitalRead(ir_car5) == LOW ? 1 : 0;
  S6 = digitalRead(ir_car6) == LOW ? 1 : 0;
}