# Controller

Simple Python script to control a 3D printer (running 
[marlin](https://marlinfw.org/meta/gcode/)) and the robot-wrist via a PS4
cotroller.

## Button Mapping

![PS4 Controller](https://forum.unity.com/proxy.php?image=https%3A%2F%2Fpbs.twimg.com%2Fmedia%2FDt6UeTLX4AEYylF.jpg&hash=47dc6a962e0fb4203613a14ac73e1910)

- Button 6 (L2): Printer Z axis up

- Button 7 (R2): Printer Z axis down

- Button 10 (L3):
    - X axis: Printer X axis
    - Y axis: Printer Y axis

- Button 11 (R3):
    - X axis: robot-wrist rotation
    - Y axis: robot-wrist arm

## Run

```
pip install -r requirements.txt
python3 controller.py <robot-wrist-serial-port> <marlin-serial-port>
```

## Resources
- https://marlinfw.org/meta/gcode/
- https://pypi.org/project/pyPS4Controller/
- https://forum.unity.com/proxy.php?image=https%3A%2F%2Fpbs.twimg.com%2Fmedia%2FDt6UeTLX4AEYylF.jpg&hash=47dc6a962e0fb4203613a14ac73e1910
