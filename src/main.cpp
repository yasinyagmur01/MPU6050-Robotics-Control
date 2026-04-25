/*
 * MPU6050 İleri Düzey Veri Akışı (Robot Kolu / IK İçin)
 * 
 * EKF (Extended Kalman Filter) hesaplanır ve 
 * LERP/IK işlemlerinin JavaScript tarafında pürüzsüz 
 * yapılabilmesi için yüksek hızda seri porttan sayısal olarak gönderilir.
 */

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

struct EKF { float x[2]; float P[2][2]; float Q[2][2]; float R[2][2]; };
Adafruit_MPU6050 mpu;
EKF ekf;
unsigned long prevTime;

void ekf_init(EKF &ekf) {
  ekf.x[0] = 0; ekf.x[1] = 0;
  ekf.P[0][0] = 1; ekf.P[0][1] = 0; ekf.P[1][0] = 0; ekf.P[1][1] = 1;
  ekf.Q[0][0] = 0.001f; ekf.Q[0][1] = 0; ekf.Q[1][0] = 0; ekf.Q[1][1] = 0.001f;
  ekf.R[0][0] = 0.03f; ekf.R[0][1] = 0; ekf.R[1][0] = 0; ekf.R[1][1] = 0.03f;
}

void ekf_predict(EKF &ekf, float gx, float gy, float gz, float dt) {
  float roll = ekf.x[0], pitch = ekf.x[1];
  float sinR = sin(roll), cosR = cos(roll), tanP = tan(pitch), cosP = cos(pitch);
  ekf.x[0] += (gx + sinR * tanP * gy + cosR * tanP * gz) * dt;
  ekf.x[1] += (cosR * gy - sinR * gz) * dt;
}

void ekf_update(EKF &ekf, float ax, float ay, float az) {
  float roll_meas = atan2(ay, az);
  float pitch_meas = atan2(-ax, sqrt(ay * ay + az * az));
  float y[2] = { roll_meas - ekf.x[0], pitch_meas - ekf.x[1] };
  for(int i=0; i<2; i++) {
    while (y[i] > PI) y[i] -= 2*PI;
    while (y[i] < -PI) y[i] += 2*PI;
  }
  ekf.x[0] += y[0] * 0.1f;
  ekf.x[1] += y[1] * 0.1f;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  if (!mpu.begin()) {
    while (1) delay(10);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  ekf_init(ekf);
  prevTime = millis();
}

void loop() {
  unsigned long now = millis();
  float dt = (now - prevTime) / 1000.0f;
  prevTime = now;
  if (dt <= 0.0f || dt > 0.5f) dt = 0.01f;

  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  ekf_predict(ekf, gyro.gyro.x, gyro.gyro.y, gyro.gyro.z, dt);
  ekf_update(ekf, accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

  float ekf_roll  = ekf.x[0] * 180.0f / PI;
  float ekf_pitch = ekf.x[1] * 180.0f / PI;

  // JS Tarafında LERP ve IK işlemlerini yapmak için ham ve hızlı veri yolluyoruz
  Serial.print(F("DATA\t"));
  Serial.print(ekf_roll, 2);
  Serial.print(F("\t"));
  Serial.println(ekf_pitch, 2);

  delay(15); // ~66 Hz süper akıcı veri
}