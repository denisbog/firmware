#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR && defined(SENSECAP_AM2302)

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "AM2302Sensor.h"
#include "TelemetrySensor.h"
#include "serialization/cobs.h"
#include <Adafruit_Sensor.h>
#include <driver/uart.h>
#include <DHT.h>

AM2302Sensor::AM2302Sensor() : TelemetrySensor(meshtastic_TelemetrySensorType_SENSOR_UNSET, "AM2302Sensor") {}

#define DHTPIN 44     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

bool AM2302Sensor::initDevice(TwoWire *bus, ScanI2C::FoundDevice *dev)
{
    LOG_INFO("%s: init", sensorName);
    dht.begin();
    return true;
}

void AM2302Sensor::setup()
{
}

bool AM2302Sensor::getMetrics(meshtastic_Telemetry *telemetry)
{
    LOG_INFO("read temperature metric");
        float h = dht.readHumidity();
        float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return false ;
    }
    telemetry->variant.environment_metrics.has_temperature = true;
    telemetry->variant.environment_metrics.temperature = t;
    telemetry->variant.environment_metrics.has_relative_humidity = true;
    telemetry->variant.environment_metrics.relative_humidity = h;
    return true;
}
#endif
