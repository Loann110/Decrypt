#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <MPU9250.h>
#include <math.h>
#include <TinyGPS++.h>

// Pins I2C pour Raspberry Pi Pico
#define SDA_PIN 16
#define SCL_PIN 17
const uint32_t SERIAL_SPEED{115200};

Adafruit_BME680 bme; // Objet pour le capteur BME680
MPU9250 mpu;         // Objet pour le capteur MPU9250

// Variables to store pitch, roll, and yaw
float pitch = 0.0;
float roll = 0.0;
float yaw = 0.0;

// Variables pour stocker les mesures
double tempC;
double humidPercent;
double pressureHpa;
double gasOhms;
double altitude;

const float cteGazParfait= 287.00;
const float graviTerrestre= 9.81;
const float seaLevel = 1013.25;

// Object creation for the TinyGPS++
TinyGPSPlus gps;

void setup() {

    //configuration UART
    Serial.begin(SERIAL_SPEED);
    Serial1.begin(9600, SERIAL_8N1, 4, 5); //UART1 : TX (GPIO4) and RX (GPIO5) Connected to the Sequencer board
    Serial2.begin(9600, SERIAL_8N1, 8, 9); //UART2 : TX (GPIO8) and RX (GPIO9) Connected to the GPS module
    
    // Configuration I2C spécifique à Raspberry Pi Pico
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);
    Wire.begin();
    delay(2000); // Délai d'initialisation de la communication I2C

    //******* INITIALIZE BME680 **********
    if (!bme.begin(0x76)) { 
        Serial.println("BME680 non détecté. Veuillez vérifier les connexions !");
        while (1);
    }

    // Configuration du capteur BME680
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_8X);
    bme.setPressureOversampling(BME680_OS_8X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // Température de chauffage à 320°C pendant 150 ms

    //******* INITIALIZE MPU9250 ********
    if (!mpu.setup(0x68)) {  
        while (1) {
            Serial.println("MPU non détecté. Veuillez vérifier les connexions !");
            delay(5000);
        }
    }

    // Calibration des capteurs
    Serial.println("Calibration de l'accéléromètre et du gyroscope...");
    Serial.println("Veuillez laisser l'appareil immobile sur une surface plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Calibration du magnétomètre...");
    Serial.println("Veuillez déplacer l'appareil en formant un 8 jusqu'à la fin.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);

    Serial.println("Initialisation terminée.");
}

float calculateAltitude(float pressure, float temp) {
    // Formule hypsométrique améliorée avec la température locale (en Kelvin)
    return log(seaLevel / pressure) * ((cteGazParfait * (temp + 273.15)) / graviTerrestre);
}

void getData() {
    // Get angles
    pitch = mpu.getPitch();
    roll = mpu.getRoll();
    yaw = mpu.getYaw();

    // Get temp, humidity, pressure, and gas resistance
    if (bme.performReading()) {
        tempC = bme.temperature;
        humidPercent = bme.humidity;
        pressureHpa = bme.pressure / 100.0; // Conversion en hPa
        gasOhms = bme.gas_resistance / 1000.0; // Conversion en kOhms
        altitude = calculateAltitude(pressureHpa,tempC);
    } else {
        Serial.println("Erreur lors de la lecture des données BME680 !");
    }
}

void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
            // Update pitch, roll, and yaw values
            getData();

            // Print the values
            print_roll_pitch_yaw();
            print_bme_data();

            prev_ms = millis();
        }
    }
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(yaw, 2);
    Serial.print(", ");
    Serial.print(pitch, 2);
    Serial.print(", ");
    Serial.println(roll, 2);
}

void print_bme_data() {
    Serial.print("Température: "); Serial.print(tempC); Serial.println(" °C");
    Serial.print("Humidité: "); Serial.print(humidPercent); Serial.println(" %");
    Serial.print("Pression: "); Serial.print(pressureHpa); Serial.println(" hPa");
    Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
    Serial.print("Gaz: "); Serial.print(gasOhms); Serial.println(" kΩ");

    delay(500); // Pause avant la prochaine mesure
}

void print_calibration() {
    Serial.println("< Paramètres de calibration >");
    Serial.println("Bias de l'accéléromètre [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();

    Serial.println("Bias du gyroscope [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();

    Serial.println("Bias du magnétomètre [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();

    Serial.println("Echelle du magnétomètre: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}