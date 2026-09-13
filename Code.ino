#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define trigPin 5
#define echoPin 18
#define irPin 19
#define servoPin 13
#define buzzerPin 23

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Servo gate;

void setup()
{
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  gate.attach(servoPin);
  gate.write(0);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SafeGate Ready");
  display.display();

  Serial.println("SafeGate Ready");
  delay(1000);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  int irValue = digitalRead(irPin);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR: ");
  Serial.println(irValue);

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  display.print("IR: ");
  display.println(irValue);

  if (distance > 0 && distance < 20)
  {
    display.println("Vehicle detected");
    display.println("Gate OPEN");

    gate.write(90);
    tone(buzzerPin, 1000, 200);
  }
  else
  {
    display.println("Area clear");
    display.println("Gate CLOSED");

    gate.write(0);
    noTone(buzzerPin);
  }

  display.display();

  delay(300);
}
