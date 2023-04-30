#ifndef calibrator_h
#define calibrator_h

template <typename Numeric, typename = typename std::enable_if<std::is_arithmetic<Numeric>::value>::type>
class Calibrator
{
public:
    /**
     * Constructor for the calibrator
     *
     * @param rawValues Array of raw values to calibrate.
     * @param calibrationValues Array of calibrated values that match the raw values.
     * @param numPoints Number of calibration points in the array.
     * @param limitOutputToCalibrationRange An optional boolean variable that indicates whether to constrain the calibrated values to the range of the calibration table. Default is 'false'
     */
    Calibrator(const Numeric *rawValues, const Numeric *calibrationValues, uint32_t numPoints, bool limitOutputToCalibrationRange = false)
    {
        // Pass the references of the input data
        _rawValues = rawValues;
        _calibrationValues = calibrationValues;
        _limitOutput = limitOutputToCalibrationRange;
        _numPoints = numPoints;
    }

    /**
     * This method checks that the data passed is usable and creates a calibration curve
     *
     * @return 'true' if successful, otherwise 'false'.
     */
    bool begin()
    {
        // Check if rawValues array is sorted in ascending order
        for (uint32_t i = 0; i < _numPoints - 1; i++)
        {
            if (_rawValues[i] > _rawValues[i + 1])
            {
                // The rawValues array is not sorted in ascending order
                return false;
            }
        }

        // Check if there are at least two calibration points
        if (_numPoints <= 1)
            return false;

        // Generate arrays for slopes and y-intercepts
        _m = new Numeric[_numPoints - 1];
        _b = new Numeric[_numPoints - 1];

        // Calculate calibration curve
        for (uint32_t i = 0; i < _numPoints - 1; i++)
        {
            _m[i] = (_calibrationValues[i + 1] - _calibrationValues[i]) / (_rawValues[i + 1] - _rawValues[i]);
            _b[i] = _calibrationValues[i] - _m[i] * _rawValues[i];
        }

        return true;
    }

    /**
     * This method calibrates a raw value against a calibration table.
     *
     * @param rawValue A raw numeric value to be calibrated.
     * @return A numeric, calibrated value.
     */
    Numeric calibrate(Numeric rawValue)
    {
        Numeric calibratedValue; // Variable für den korrigierten Wert

        if (_m == nullptr || _b == nullptr)
            return rawValue;

        // Ist der Wert außerhalb des Bereiches?
        if (rawValue < _rawValues[0]) // Prüfe ob Rohwert kleiner als der erste Kalibrierpunkt ist
        {
            if (_limitOutput)
                return _calibrationValues[0];

            calibratedValue = _m[0] * rawValue + _b[0]; // Anwenden der Kalibrierfunktion
            return calibratedValue;
        }
        else if (rawValue > _rawValues[_numPoints - 1]) // Prüfe ob Rohwert größer als der letzte Kalibrierpunkt ist
        {
            if (_limitOutput)
                return _calibrationValues[_numPoints - 1];

            calibratedValue = _m[_numPoints - 2] * rawValue + _b[_numPoints - 2]; // Anwenden der Kalibrierfunktion
            return calibratedValue;
        }

        // Kalibrierfunktion
        for (uint32_t i = 0; i < _numPoints - 1; i++) // Durchlaufe die gegebenen Kalibrierpunkte
        {
            if (rawValue >= _rawValues[i] && rawValue <= _rawValues[i + 1]) // Prüfe ob Rohwert zwischen zwei Kalibrierpunkten liegt
            {
                calibratedValue = _m[i] * rawValue + _b[i]; // Anwenden der Kalibrierfunktion
                return calibratedValue;
            }
        }

        // Wurde keine Kalibrierung angewendet, wird der Rohwert zurückgegeben
        return rawValue;
    }

private:
    const Numeric *_rawValues;         // Known input values
    const Numeric *_calibrationValues; // Known calibration values
    uint32_t _numPoints;               // Number of calibration points
    Numeric *_m;                       // Array for gradients
    Numeric *_b;                       // Array for y-intercepts
    bool _limitOutput;                 // Limit output to calibration range if 'true'
};

#endif

/* // Demo
float voltages[] = {3300, 3750, 3800, 3880, 4100, 4200};   // Batteriespannung
float capacities[] = {0.0, 10.0, 40.0, 65.0, 90.0, 100.0}; // Restkapazität in Prozent

float in_humidity[] = {35.6, 55.7, 75.2};   // Ausgabe des Feuchtesensors
float cal_humidity[] = {33.3, 50.2, 77.8};  // Kalibrierte Werte des Feuchtesensors

int main()
{
    //** LiPo-Bateriespannung zu Kapazität mappen
    // Initialisierung des Kalibrators für die Batteriespannung ohne Interpolation außerhalb des Kalibrierbereiches
    uint32_t numPoints = sizeof(voltages) / sizeof(voltages[0]);
    if (!init(voltages, capacities, numPoints, true))
    {
        std::cout << "Voltage calibrator init failed\n";
        return 1;
    }
    // Berechnung und Ausgabe
    std::cout << "Battery\n";
    std::cout << "Volt\tPercent\n";
    for (uint8_t i = 0; i < 15; i++)
    {
        float battVolt = (float)(i + 30) * 100.0f;
        float battPercent = calibrate(battVolt); // Kalibrierung des Eingangswertes
        std::cout << battVolt;
        std::cout << "\t";
        std::cout << battPercent;
        std::cout << "\n";
    }

    //** Feuchtesensor mappen
    // Initialisierung des Kalibrators für die Feuchtewerte mit Interpolation außerhalb des Kalibrierbereiches
    uint32_t numPoints2 = sizeof(in_humidity) / sizeof(in_humidity[0]);
    if (!init(in_humidity, cal_humidity, numPoints2, false))
    {
        std::cout << "Humidity calibrator init failed\n";
        return 1;
    }
    // Berechnung und Ausgabe
    std::cout << "\nHumidity\n";
    std::cout << "in\tout\n";
    for (uint8_t i = 0; i < 11; i++)
    {
        float inHum = (float)i * 10.0f;
        float outHum = calibrate(inHum); // Kalibrierung des Eingangswertes
        std::cout << inHum;
        std::cout << "\t";
        std::cout << outHum;
        std::cout << "\n";
    }

    return 0;
}
 */