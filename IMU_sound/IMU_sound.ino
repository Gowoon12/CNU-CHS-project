#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int buzzerPin = 8;
float filteredAngleX = 0.0;
float alpha = 0.9;  // 필터 계수: 0.9 ~ 0.98 정도가 일반적, 값을 그대로 쓰면 너무 노이즈가 심해서 간단한 필터 적용함. 

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
    // analogWrite(buzzerPin, 50); // PWM으로 소리 크기 조절 0~255 만약 digitalWrite(buzzerPin, HIGH); 소리가 너무 크면 위에꺼 주석처리하고 이 줄 사용하기. 
    // 저역통과 필터 적용
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(10);  // 조금 더 느려도 괜찮으면 이 값을 늘릴 수도 있음, 또는 모터 값이 너무 업데이트가 빨라서 떨리면 이 값을 키워도 좋음. 
}
