# MPU6050 Project: From Sensor to Simulation

This project is a journey of taking data from a noisy, short-cabled ("bad setup") MPU6050 accelerometer/gyroscope sensor and transforming it into professional, smooth, and entertaining interfaces using mathematical filters (EKF, EMA, DEMA, LERP).

## 🚀 Live Demos (Runs in Browser)
- [🤖 Robotic Arm Controller (IK)](https://yasinyagmur01.github.io/MPU6050-Robotics-Control/robot_arm.html)
- [🎮 Combat Flight Simulator](https://yasinyagmur01.github.io/MPU6050-Robotics-Control/combat_flight.html)
- [📊 Cockpit & Data Visualizer](https://yasinyagmur01.github.io/MPU6050-Robotics-Control/visualizer.html)

## 🔌 Hardware Setup (To Recreate at Home)
When setting up the circuit at home, connect the cables as follows:
* **VCC:** Arduino `5V`
* **GND:** Arduino `GND`
* **SDA:** Arduino `A4`
* **SCL:** Arduino `A5`

## 🚀 Project Phases & What We Built

Throughout the project, we developed 3 main concepts. To run them locally, plug in your Arduino, open VS Code, type `python -m http.server 8080` in the terminal, and click the links in your browser.

### 1. Airplane Cockpit and 3D Visualizer (`visualizer.html`)
**What We Did:** Filtered raw sensor data using the "Extended Kalman Filter (EKF)" to control a 3D airplane on the screen.
**Features:** Realistic Artificial Horizon, Roll and Pitch indicators, real-time temperature tracking.

### 2. Combat Flight Game (`combat_flight.html`)
**What We Did:** Used the sensor as a joystick. Since the sensor was very noisy and the cable was short, we made the hitboxes extremely large.
**Features:** A flat green terrain, auto-firing lasers, enemy planes coming towards you and shooting, giant meteors to dodge. You control the plane by tilting the sensor up and down.

### 3. Inverse Kinematics (IK) Robotic Arm Simulation (`robot_arm.html`)
**What We Did:** Responding to the request for "a robotic arm, but soft and mathematical," we wrote pure engineering algorithms.
**Mathematics and Features Used:**
* **DEMA (Double Exponential Moving Average):** A heavily weighted 2-layer signal filter to eliminate 100% of sensor vibrations. Even if there's an earthquake, the robot won't flinch.
* **Derivative (Speed) Detection:** Calculates your instantaneous speed to determine which axis you are moving on, lighting up the corresponding "Neon" indicator on the interface (X-Z or Y axis).
* **Inverse Kinematics (Trigonometry):** Takes the sensor position as a target (Target XYZ). It automatically calculates how many degrees the shoulder and elbow (`θ1, θ2, θ3`) need to bend using the Law of Cosines for the robotic arm to reach that target.
* **LERP (Linear Interpolation):** The arm doesn't instantly snap to calculated angles. With a heavy coefficient like t=0.015, it glides smoothly to the target as if it has physical mass.

---

## 🛠️ How to Run Locally?

1. Plug the Arduino USB cable into your computer.
2. **Close all open Serial Monitors** (Make sure the port is not busy).
3. Open a terminal (CMD/PowerShell/VS Code Terminal) in the folder and enter this command: 
   `python -m http.server 8080`
4. Open your browser and type the address of the project you want to see:
   * **Robotic Arm:** `http://localhost:8080/robot_arm.html`
   * **Combat Game:** `http://localhost:8080/combat_flight.html`
   * **Airplane Cockpit:** `http://localhost:8080/visualizer.html`
5. Once the page opens, click the **SYS.CONNECT()** (or Connect to Sensor) button on the top left and select your `COM` port.
6. Have fun!

*Note: If you want to modify the code and upload it to the Arduino again, you must close that browser tab before uploading, otherwise you will get a "COM Port Access Denied" error.*
