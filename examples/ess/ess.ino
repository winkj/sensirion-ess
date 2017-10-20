#include <sensirion_ess.h>

SensirionESS ess;

void setup()
{
  Serial.begin(9600);
  delay(1000); // let console settle

  if (ess.initSensors() != 0) {
      Serial.print("Error while initializing sensors: ");
      Serial.print(ess.getError());
      Serial.print("\n");
  }
}

void loop() {
  if (ess.isInitialized()) {
    float temp, rh, tvoc = -1;

    if (ess.measureRHT() != 0) {
      Serial.print("Error while measuring RHT: ");
      Serial.print(ess.getError());
      Serial.print("\n");
    } else {
      temp = ess.getTemperature();
      rh = ess.getHumidity();
    }

    if (ess.measureIAQ() != 0) {
      Serial.print("Error while measuring IAQ: ");
      Serial.print(ess.getError());
      Serial.print("\n");
    } else {
      tvoc = ess.getTVOC();
    }

    Serial.print(temp);
    Serial.print(" ");
    Serial.print(rh);
    Serial.print(" ");
    Serial.print(tvoc);
    Serial.print(" ");
    Serial.print("\n");

    delay(ess.remainingWaitTimeMS());
  } else {
    Serial.print("ESS not initialized. Trying to reinitialize");
    Serial.print("\n");
    if (ess.initSensors() != 0) {
        Serial.print("Error while initializing sensors: ");
        Serial.print(ess.getError());
        Serial.print("\n");
    }
    delay(1000);
  }
}
