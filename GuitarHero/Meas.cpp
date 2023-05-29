#include "Meas.h" 
SFE_MMC5983MA myMag;

void Meas::SensorSetup()
  {
    if (!IMU.begin()) 
    {
      Serial.println("You. Have. No. Good. IMU. Reading.");

      while (1);
    }
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    s.reserve(200);
    Wire.begin();

    if (myMag.begin() == false)
    {
      Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
      while (1)
            ;
    }
    myMag.softReset();
    Serial.println("MMC5983MA connected");
    int celsius = myMag.getTemperature();
    Serial.println("Calibrating..."); // Calibrating by averaging over 100 samples. System must remain still during this.
    
    for (int i = 0; i < 100; ++i)
    {       
        Magnetometer();
        MagYSum += normalizedY;
        MagXSum += normalizedX;
    }
    MagYSum = MagYSum/100;
    MagXSum = MagXSum/100;
    MagCalibratedAngle = atan(MagXSum/MagYSum);
    Serial.println("Calibration Completed!");
  }

void Meas::Gyroscope()
{
  if (IMU.gyroscopeAvailable()) 
    {
    IMU.readGyroscope(Gyro_x, Gyro_y, Gyro_z);
    GyrOmega = Gyro_z;
    }
    else
    {
      Serial.println("Gyroscope not Available"); 
      while (1)
            ;
    }   
}

void Meas::Accelerometer()
  {
    if (IMU.accelerationAvailable()) 
    {
      IMU.readAcceleration(Acc_x, Acc_y, Acc_z);
      AccAngle = Acc_x*(-90);
    }
    else
    {
      Serial.println("Accelerometer not Available"); 
      while (1)
            ;
    }   
  }
  
void Meas::Magnetometer()
  {
    currentX = myMag.getMeasurementX();
    currentY = myMag.getMeasurementY();
    currentZ = myMag.getMeasurementZ();
    normalizedX = (double)currentX - 131072.0;
    normalizedX /= 131072.0;
    normalizedY = (double)currentY - 131072.0;
    normalizedY /= 131072.0;
    normalizedZ = (double)currentZ - 131072.0;
    normalizedZ /= 131072.0;
    MagAngle = (atan(normalizedX/normalizedY)-MagCalibratedAngle)*(180/PI); // We find the magnetometer angle as the difference between the 
  }                                                                         // calibrated angle (assumed to be zero), and the angle obtained here

BLA::Matrix<2,1> Meas::GetMeasurements()
  {
    AngularPosition = (AccAngle+MagAngle)/2; // Here the average position is computed from the Accelerometer and the magnetometer.
    BLA::Matrix<2,1> Z = {GyrOmega, AngularPosition}; // Vector containing angular velocity and angular position.
    return Z;
  }



