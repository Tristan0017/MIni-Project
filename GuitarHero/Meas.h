#ifndef Meas_H_
#define Meas_H_
#include <BasicLinearAlgebra.h>
using namespace BLA;
#include <Arduino_LSM6DS3.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <cmath> 
#include <Arduino.h>
#include <Wire.h>
#include <string>

class Meas
{
public:

void SensorSetup();

void Gyroscope();

void Accelerometer();

void Magnetometer();

BLA::Matrix<2,1> GetMeasurements();

private:
    std::string s;       // We reserve some data entries for accelerometer data.
    float AngularPosition { 0 };
    float AccAngle { 0 };
    float MagAngle { 0 };
    float GyrOmega { 0 };
    float Acc_x { 0 };
    float Acc_y { 0 };
    float Acc_z { 0 };
    float Gyro_x { 0 };
    float Gyro_y { 0 };
    float Gyro_z { 0 };
    float theta { 0 };
    float phi { 0 };
    float psi { 0 };
    uint32_t currentX { 0 };
    uint32_t currentY { 0 };
    uint32_t currentZ { 0 };
    double normalizedX { 0 };
    double normalizedY { 0 };
    double normalizedZ { 0 };
    float MagCalibratedAngle { 0 };  
    float MagYSum { 0 };
    float MagXSum { 0 };
};
#endif