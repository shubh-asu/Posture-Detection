# Posture Detection using Arduino BLE 33
This project detects body postures like supine, prone, side, sitting, and unknown on the Arduino using Tensorflow-Lite. 


## Installation and Changes in Code to use it

You'll have to install the Python Tkinter and pyserial libraries on your laptop to run the GUI and will need to install Harvard Arduino Tensorflow libraries to compile the Arduino code. 

Also, change the model path in the Arduino file and specify the correct COM port in the app.py file to connect the interface and Arduino.

## Demonstration Images
Selected Accelerometer Sensor
![Selected Accelerometer Sensor](./images/AccDetect.jpeg)

Posture predicted using Accelerometer
![Posture predicted using accelerometer](./images/AccDisp.jpeg)

Selected Magnetometer Sensor
![Selected Magnetometer Sensor](./images/MagDetect.jpeg)

Posture predicted using Magnetometer
![Posture predicted using magnetometer](./images/MagDisp.jpeg)
