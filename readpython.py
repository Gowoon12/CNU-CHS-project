import serial
import time

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1)
time.sleep(2)  # 연결 안정화 대기

print("기울기 X 값을 수신 중...")

while True:
    try:
        if arduino.in_waiting > 0:
            line = arduino.readline().decode('utf-8').strip()
            if line:
                angle_x = float(line)
                print(f"{angle_x:.2f}")
    except Exception as e:
        print(f"오류: {e}")
