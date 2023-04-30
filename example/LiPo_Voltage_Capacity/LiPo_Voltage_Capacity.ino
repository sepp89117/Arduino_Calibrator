/*
 * Example of using the calibrator to non-linearly map the battery voltage of a LiPo battery to its remaining capacity
 * Author: Sebastian Balzer
 * Date: 2023-04-30
 */

#include <calibrator.h>

//** Calibrator input values (measurement)
// Note 1: The input values must be sorted in ascending order
// Note 2: The input and output values must have the same data type and the same length. Choose the data type depending on what type of data you want as a calibration result
const float voltages[] = {3300, 3750, 3800, 3880, 4100, 4200}; // Battery voltages in mV

//** Calibrator output values (calibration values)
// Note: Output values must have the same length as input values
const float capacities[] = {0, 10, 40, 65, 90, 100}; // Remaining capacity at voltage in %

//** Length of the arrays
// The length must be greater than or equal to 2
uint32_t valuesLen = 6;

//** Initiate the Calibrator
// Pass the data type of your values in the angle brackets <>! The data type must be numeric!
// Define with the boolean whether the output should be limited to the calibration range (optional, default is false). If TRUE, the nearest calibration value is returned. If FALSE, the nearest slope and intercept are used for the calibration.
// - In this example the value is 'true' because theoretically we can't have less than 0% capacity and no more than 100% capacity
Calibrator<float> battCalibrator(voltages, capacities, valuesLen, true);

void setup()
{
    // Serial for the output of this example
    Serial.begin(19200);

    // Call 'begin()' to calculate the slopes and intercepts for the calibration, otherwise the raw value is returned from the 'calibrate()' method
    if (!battCalibrator.begin())
    {
        Serial.println("Calibrator initialization failed!");
        while (1)
            ;
    }
    else
    {
        Serial.println("Voltage [mV]\tRemaining capacity [%]");
    }
}

void loop()
{
    // Get the battery voltage with the appropriate function! Here 'random()' is used for the universal example
    float battVoltage = random(3200.0f, 4300.0f);

    // Calibrate the battery voltage to the percentage of remaining capacity
    float remainCap = battCalibrator.calibrate(battVoltage);

    Serial.print(battVoltage, 2);
    Serial.print("\t\t");
    Serial.println(remainCap, 2);

    delay(1000);
}