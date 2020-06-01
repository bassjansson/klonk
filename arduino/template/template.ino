//=================================//
//========== Pin Defines ==========//
//=================================//

// Input pins
#define POT_PIN  A0

// Output pins
#define LED_PIN  13


//==================================//
//========== Data Defines ==========//
//==================================//

#define DATA_DELIMITER  ','
#define DATA_ENDLINE    '\n'


//==================================//
//========== Control Data ==========//
//==================================//

#define CONTROL_DATA_SIZE  3  // [Control, Value1, Value2]
#define CONTROL_LED        100

uint32_t controlData[CONTROL_DATA_SIZE];
uint16_t controlDataIndex = 0;
String controlDataString = "";


//=================================//
//========== Sensor Data ==========//
//=================================//

#define SENSOR_DATA_SIZE  1  // 1 POT meter
#define SENSOR_POT        200

uint32_t sensorData[SENSOR_DATA_SIZE];
unsigned long previousSendTime = 0;


//==================================//
//========== Setup & Loop ==========//
//==================================//

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);

    // Initialize digital pins
    pinMode(LED_PIN, OUTPUT);

    // Initialize control and sensor data
    initControlData();
    initSensorData();
}

void loop()
{
    // Read control data
    readControlData();

    // Read and send POT meter value every quarter second
    unsigned long currentSendTime = millis();
    if (currentSendTime - previousSendTime >= 250)
    {
        previousSendTime = currentSendTime;

        sendSensorValue(SENSOR_POT, analogRead(POT_PIN));
    }

    // Delay a little bit for stability
    //if (!Serial.available()) // TODO
    delay(25);
}


//=================================//
//========== Sensor Data ==========//
//=================================//

void initSensorData()
{
    for (uint16_t i = 0; i < SENSOR_DATA_SIZE; ++i)
        sensorData[i] = 0;
}

void sendSensorValue(uint16_t sensor, uint32_t value)
{
    if (sensor >= SENSOR_DATA_SIZE)
        return;

    if (sensorData[sensor] == value)
        return;

    sensorData[sensor] = value;

    Serial.print(sensor);
    Serial.print(DATA_DELIMITER);
    Serial.print(value);
    Serial.print(DATA_ENDLINE);
}


//==================================//
//========== Control Data ==========//
//==================================//

void initControlData()
{
    for (uint16_t i = 0; i < CONTROL_DATA_SIZE; ++i)
        controlData[i] = 0;
}

void onControlDataReceived(uint32_t data[CONTROL_DATA_SIZE])
{
    switch (data[0])
    {
        case CONTROL_LED:
            digitalWrite(LED_PIN, data[1] > 0);
            break;

        default: break;
    }
}

void readControlData()
{
    while (Serial.available()) // TODO: 'if' instead?
    {
        char c = Serial.read();

        if (c == DATA_DELIMITER || c == DATA_ENDLINE)
        {
            if (controlDataIndex < CONTROL_DATA_SIZE)
            {
                if (controlDataString.length() > 0)
                    controlData[controlDataIndex] = controlDataString.toInt();
                else
                    controlData[controlDataIndex] = 0;

                controlDataIndex++;
                controlDataString = "";
            }

            if (c == DATA_ENDLINE)
            {
                onControlDataReceived(controlData);

                for (uint16_t i = 0; i < CONTROL_DATA_SIZE; ++i)
                    controlData[i] = 0;

                controlDataIndex = 0;
            }
        }
        else
        {
            if (controlDataIndex < CONTROL_DATA_SIZE)
                controlDataString += c;
        }
    }
}
