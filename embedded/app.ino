#include <math.h>
#include <SoftwareSerial.h>

#define LED 8

SoftwareSerial ESP8285(10, 11); // RX, TX
String wifi_ssid = "NETGEAR54";
String wifi_pass = "sweetchair5";

const int B = 3985;    // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0;
const int pinLightSensor = A1;
const int pinMoistureSensor = A2;

float temperature;
float moisture;
float light;

void setup()
{
    pinMode(LED, OUTPUT);

    Serial.begin(9600);
    ESP8285.begin(115200);
    initESP8285();
}

void loop()
{
    int a = analogRead(pinTempSensor);
    int b = analogRead(pinLightSensor);
    int c = analogRead(pinMoistureSensor);

    float R = (float)(1023 - a) * R0 / a;

    temperature = 1 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet
    light = b;                                                 // get lumens value
    moisture = map(c, 0, 950, 0, 100);                         // get % of humidity

    if (temperature < 8 || moisture < 30 || !ESP8285.available())
    {
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        delay(500);
    }

    Serial.print("temp = ");
    Serial.println(temperature);

    Serial.print("light = ");
    Serial.println(light);

    Serial.print("moisture = ");
    Serial.println(moisture);

    if (ESP8285.available())
        Serial.println(ESP8285.readString());
    else
        Serial.println("Wifi not available");

    delay(1000);
}

void initESP8285()
{
    Serial.println("Begin initialisation...");
    delay(500);
    send_cmd("AT+RST");
    receive_cmd(2000);
    delay(500);
    send_cmd("AT+CWMODE=3");
    receive_cmd(5000);
    delay(1500);
    send_cmd("AT+CWJAP=\"" + wifi_ssid + "\",\"" + wifi_pass + "\"");
    receive_cmd(10000);
    delay(500);
    send_cmd("AT+CIFSR");
    receive_cmd(1000);
    delay(500);
    send_cmd("AT+CIPMUX=1");
    receive_cmd(1000);
    send_cmd("AT+CIPSERVER=1,80");
    receive_cmd(1000);
    delay(500);
    Serial.println("...Finifhed initialization !");
    Serial.println("");
    delay(1500);
    Serial.println("Local IP is : ");
    send_cmd("AT+CIFSR");
    receive_cmd(1000);
}

/* Les différentes commandes sont trouvables ici : 
https://www.itead.cc/wiki/ESP8266_Serial_WIFI_Module
*/

void send_cmd(String commande)
{
    Serial.println(commande);
    ESP8285.println(commande);
}

void receive_cmd(const int timeout)
{
    String reponse = "";
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (ESP8285.available())
        {
            char c = ESP8285.read();
            reponse += c;
        }
    }

    Serial.print(reponse);
}