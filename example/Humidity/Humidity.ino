/*
 * Example of using the calibrator for non-linear mapping of humidity readings from a humidity measurement sensor
 * Author: Sebastian Balzer
 * Date: 2023-04-30
 */

#include <calibrator.h>

//** Calibrator input values (measurement)
// Note 1: The input values must be sorted in ascending order
// Note 2: The input and output values must have the same data type and the same length. Choose the data type depending on what type of data you want as a calibration result
const float in_humidity[] = {35.6, 55.7, 75.2};   // Readings from the humidity sensor in %rH

//** Calibrator output values (calibration values)
// Note: Output values must have the same length as input values
const float cal_humidity[] = {33.3, 50.2, 77.8};  // Calibrated values of the humidity sensor in %rH

//** Length of the arrays
// The length must be greater than or equal to 2
uint32_t valuesLen = 3;

//** Initiate the Calibrator
// Pass the data type of your values in the angle brackets <>! The data type must be numeric!
// Define with the boolean whether the output should be limited to the calibration range (optional, default is false). If TRUE, the nearest calibration value is returned. If FALSE, the nearest slope and intercept are used for the calibration.
// - In this example the value is 'false' because we want the values 0 and 100% to be achievable as well
Calibrator<float> humCalibrator(in_humidity, cal_humidity, valuesLen, false);

void setup()
{
    // Serial for the output of this example
    Serial.begin(19200);

    // Call 'begin()' to calculate the slopes and intercepts for the calibration, otherwise the raw value is returned from the 'calibrate()' method
    if (!humCalibrator.begin())
    {
        Serial.println("Calibrator initialization failed!");
        while (1)
            ;
    }
    else
    {
        Serial.println("Input [%rH]\tOutput [%rH]");
    }
}

void loop()
{
    // Get the reading with the corresponding function! Here 'random()' is used for the universal example
    float humidity = random(0, 100);

    // Calibrate the battery voltage to the percentage of remaining capacity
    float calibratedValue = humCalibrator.calibrate(humidity);

    Serial.print(humidity, 2);
    Serial.print("\t\t");
    Serial.println(calibratedValue, 2);

    delay(1000);
}