#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int buzzerPin = 8;
float filteredAngleX = 0.0;
float alpha = 0.9;  // 필터 계수: 0.9 ~ 0.98 정도가 일반적

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Mega: SDA=20, SCL=21
  mpu.initialize();

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  if (mpu.testConnection()) {
    Serial.println("MPU6050 연결 성공");
  } else {
    Serial.println("MPU6050 연결 실패");
    while (1);
  }
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float rawAngleX = atan2(ay, az) * 180 / PI;

  // 저역통과 필터 적용
  filteredAngleX = alpha * filteredAngleX + (1 - alpha) * rawAngleX;

  // Serial.print("필터된 기울기 X: ");
  Serial.println(filteredAngleX);

  if (filteredAngleX < 60) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(10);  // 조금 더 느려도 괜찮으면 이 값을 늘릴 수도 있음
}
