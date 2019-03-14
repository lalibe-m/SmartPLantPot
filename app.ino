#include <math.h>

#define LED 2

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
const int pinLightSensor = A1;    // Grove - Light Sensor connect to A1
const int pinMoistureSensor = A2; // Grove - Moisture Sensor connect to A1

void setup()
{
    // initialize the digital pin2 as an output.
    pinMode(LED, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    int a = analogRead(pinTempSensor);
    int b = analogRead(pinLightSensor);
    int c = analogRead(pinMoistureSensor);

    float R = 1023.0 / a - 1.0;
    R = R0 * R;

    float temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet
    float light = b;                                                   // get lumens value
    float moisture = c / 100;

    if (temperature < 8 || moisture < 30)
    {
        digitalWrite(LED, HIGH); // set the LED on
        delay(500);              // for 500ms
        digitalWrite(LED, LOW);  // set the LED off
        delay(500);
    }

    Serial.print("temp = ");
    Serial.println(temperature);

    Serial.print("light = ");
    Serial.println(light);

    Serial.print("moisture = ");
    Serial.println(light);

    delay(1000);
}