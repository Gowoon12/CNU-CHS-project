import os
from motorLLC_sync import *
from dynamixel_sdk import * 
import time  # Make sure to import time for sleep

if os.name == 'nt':
    import msvcrt
    def getch():
        return msvcrt.getch().decode()
else:
    import sys, tty, termios
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    def getch():
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

mc = motorLLC()
mc.open()
mc.torque_enable()

# The positions you want to move the motor to

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=1)
time.sleep(2)  # 연결 안정화 대기
print("기울기 X 값을 수신 중...")
print("Press 'esc' to quit the program.")

while True:
    try:
        if arduino.in_waiting > 0:
            line = arduino.readline().decode('utf-8').strip()
            if line:
                angle_x = float(line)
                mc.moveTo([int(angle_x*100)])
                print(f"{angle_x:.2f}")
                #time.sleep(0.1)  # Wait for 1 second
    except Exception as e:
        print(f"오류: {e}")

# Close the motor connection
mc.torque_disable()
mc.close()
