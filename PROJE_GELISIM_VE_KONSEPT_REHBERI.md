# MPU6050 Project: Step-by-Step Development, Background Concepts, and Research Guide

This document contains a guide on how our project with the Arduino UNO and MPU6050 (Accelerometer and Gyroscope) sensor **developed step-by-step**, the **mathematical/physical concepts** running in the background, and **internet research topics** for any points you might be missing. It is designed as a guide for you to read at home and fully grasp the logic of the project.

---

## Phase 1: The Beginning and the Reality of Bad Hardware
**What We Did:**
We connected the sensor to the Arduino and started reading acceleration (X, Y, Z) and angular velocity data via the Serial Monitor. However, there was a major physical problem: **The sensor was extremely jittery (noisy), and your cable was short.**

**What Was Happening in the Background? (Internet Research: Sensor Noise)**
The MPU6050 consists of two main sensors:
1. **Accelerometer:** It measures gravity to know direction (which way is down) very well. However, it is hyper-sensitive to movement and vibration. Even tapping the table makes the values jump (This is called "Noise").
2. **Gyroscope:** It measures rotational speed. It measures short-term movements very smoothly and flawlessly. But over time, errors accumulate, and it generates values as if it's rotating even when standing still (This is called "Drift").

---

## Phase 2: Filtering and Airplane Cockpit (Visualizer)
**What We Did:**
Instead of directly using that terrible, jittery data from the sensor, we wrote a filter and connected it to a 3D Airplane Cockpit (Artificial Horizon) interface called `visualizer.html`. Using the Web Serial API, we allowed the browser to talk directly to the Arduino.

**What Was Happening in the Background? (Internet Research: Kalman Filter - EKF)**
To read the sensor properly, we used the **Extended Kalman Filter (EKF)** algorithm. The Kalman filter performs "Sensor Fusion":
* **Predict:** It guesses where the airplane will be in the next moment by looking at the gyroscope (because the gyroscope is great at instantaneous changes).
* **Update:** It corrects the "drift" error made by the gyroscope over time by looking at the accelerometer (gravity).
* **Result:** The airplane you see on the screen rotates quite smoothly, even if your hand shakes.

---

## Phase 3: Gamification (Combat Flight Game)
**What We Did:**
Instead of just watching the horizon with the sensor, we turned it into a joystick. We made a WebGL space/combat game called `combat_flight.html`.

**What Was Happening in the Background?**
When your sensor is bad and your movement area is restricted, you can't play "precision aiming" games. Therefore, while coding the background, we set the **Hitboxes (the hit areas of enemy planes and yours) to massive sizes**. We also added automatic firing. This guaranteed that you would enjoy the game and hit targets even if the sensor jittered.

---

## Phase 4: Mathematical Transformation (Converting Signal to Digital)
**What We Did:**
You wanted to step away from game logic and turn to mechanics. You said, "When the robotic arm tilts this much, it should turn right; when it does this, it should fire a missile."

**What Was Happening in the Background? (Deadzone and Hysteresis Concepts)**
We had to convert an analog sensor with a short cable into a "Digital button":
1. **Deadzone:** We considered all vibrations occurring when the sensor is centered (between -5 and +5 degrees) as "Zero" in the code. Even if your hand shook, the robot stood still.
2. **Hysteresis (Schmitt Trigger):** Nothing happened until you tilted the sensor 15 degrees; once past 15 degrees, the **ROBOT TURN RIGHT** command fired suddenly like a "Click!". When returning, it didn't stop turning at 15 degrees, but continued until it dropped to 5 degrees. This way, we prevented bouncing (jittering like "Open-Close-Open-Close" at the threshold) that would burn out a motor.

---

## Phase 5: Inverse Kinematics and Robotic Arm Simulation
**What We Did:**
You said, "I don't want digital, rigid movements; I want an engineering interface with soft transitions and mathematical/physical indicators." Upon this, we built the `robot_arm.html` interface and coded it so hundreds of data points per second would flow from the Arduino.

**What Was Happening in the Background? (Internet Research: Advanced Robotics)**

1. **DEMA (Double Exponential Moving Average):**
   To make the sensor "ultra-soft," we added a data-upon-data filter (DEMA). This filter made even your sudden movements heavy and fluid, as if a steel robotic arm weighing tons was moving.

2. **Inverse Kinematics (IK):**
   There are two methods in robotics:
   * *Forward Kinematics (FK):* If I set the shoulder angle to 30 and the elbow to 45 degrees, where does the tip of the robot go? (Easy to calculate).
   * *Inverse Kinematics (IK):* I want the tip of the robot to go exactly to coordinates X=10, Y=15; **how many degrees must the shoulder and elbow be?** (Very difficult to calculate).
   In the final phase of the project, we embedded an IK engine into the interface. Your sensor only provided the X and Y target; in the background, trigonometry (Law of Cosines) worked to calculate the shoulder (`Theta 2`) and elbow (`Theta 3`) angles and bent the robot to that point on the screen.

3. **LERP (Linear Interpolation):**
   To prevent the robot from instantly snapping (teleporting) to the calculated angles, we used the LERP formula. Every second, the robot advanced by 5% toward the target angle, which created that magnificent gliding (soft transition) sensation.

---

## Summary and Conclusion
In situations where we cannot change the physical conditions (hardware), we experienced step-by-step how we can mask these physical errors and create perfect systems with the power of software (Filters, Hysteresis, Kinematic algorithms).

After reading this guide, you can easily run and examine all these interfaces at home by following the run instructions in the `README.md` file.
