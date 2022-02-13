import threading
import time
import serial
import sys

from pyPS4Controller.controller import Controller

class MyController(Controller):
    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)

    L3 = [0, 0]
    R3 = [0, 0]
    R2 = 0
    L2 = 0

    def on_L3_up(self, value):
        self.on_L3_y(value)

    def on_L3_down(self, value):
        self.on_L3_y(value)

    def on_L3_y_at_rest(self):
        self.on_L3_y(0)

    def on_L3_left(self, value):
        self.on_L3_x(value)

    def on_L3_right(self, value):
        self.on_L3_x(value)

    def on_L3_x_at_rest(self):
        self.on_L3_x(0)

    def on_R3_up(self, value):
        self.on_R3_y(value)

    def on_R3_down(self, value):
        self.on_R3_y(value)

    def on_R3_y_at_rest(self):
        self.on_R3_y(0)

    def on_R3_left(self, value):
        self.on_R3_x(value)

    def on_R3_right(self, value):
        self.on_R3_x(value)

    def on_R3_x_at_rest(self):
        self.on_R3_x(0)

    def on_L3_x(self, value):
        self.L3[0] = (value/32767)

    def on_L3_y(self, value):
        self.L3[1] = (value/32767)

    def on_R3_x(self, value):
        self.R3[0] = (value/32767)

    def on_R3_y(self, value):
        self.R3[1] = (value/32767)

    def on_R2_press(self, value):
        self.R2 = (value/32767) + 1

    def on_R2_release(self):
        self.R2 = 0

    def on_L2_press(self, value):
        self.L2 = (value/32767) + 1

    def on_L2_release(self):
        self.L2 = 0

controller = MyController(interface="/dev/input/js0",
    connecting_using_ds4drv=False)

controller_thread = threading.Thread(target=controller.listen)
controller_thread.start()

ser1 = serial.Serial(sys.argv[1])
ser2 = serial.Serial(sys.argv[2], 115200)

rot_speed = 45 # °/s
arm_speed = 45 # °/s
x_speed = 30 # mm/s
y_speed = 30 # mm/s
z_speed = 2 # mm/s

rot = 0
arm = 0
x = 0
y = 0
z = 0
dt = 0.10

def send_gcode(txt):
    ser1.write((txt + '\n').encode())
    ser2.write((txt + '\n').encode())
    print(txt)

send_gcode('W1 0 0')
send_gcode('G92 E0 X0 Y0 Z0')

while True:
    rot += controller.R3[0] * rot_speed * dt
    arm += controller.R3[1] * arm_speed * dt
    x += controller.L3[0] * x_speed * dt
    y += controller.L3[1] * y_speed * dt
    z += (controller.L2 - controller.R2) * z_speed * dt

    send_gcode('W0 %f %f' % (rot, arm))
    send_gcode('G0 X%f Y%f Z%f' % (x, y, z))

    time.sleep(dt)
