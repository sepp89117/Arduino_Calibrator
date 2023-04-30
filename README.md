# Calibrator

Arduino library for calibrating nonlinear values


## Description

With this library, raw numerical values can be calibrated against a calibration table.
It is similar to the Arduino 'map()' method, but with an unlimited number of values to map.

## Special feature
A Boolean value can be used to define whether the output should be limited to the calibration range (optional, default is 'false').
If true, the closest calibration value is returned. If false, the nearest slope and intercept are used for calibration.

## Usage
See the examples for details