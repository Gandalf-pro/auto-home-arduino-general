#ifndef BlindsFeature__
#define BlindsFeature__

#include <General.hpp>
int down1per = 235;
int up1per = 260;
unsigned long timeOutTime = 35 * 1000;

namespace home {
class BlindsFeature : public Feature {
   private:
    /* data */
    uint8_t blindsLevel = 0;
    int gotoLevel = -1;

    // Pins
    uint8_t relayUpPin;
    uint8_t relayDownPin;
    uint8_t switchPin;
    uint8_t upButtonPin;
    uint8_t downButtonPin;

    unsigned long timePassed;
    unsigned long reachedZeroLast = 0;

    // returns the passed time as percent return 666 if succesfully finished
    int waitForMil(unsigned long wait, bool up) {
        unsigned long current = millis();
        unsigned long initial = millis();
        ;
        Serial.printf("Waiting for %ld milis\n", wait);

        // delay(wait);
        // for (;current - initial <= wait;current = millis()){
        //   Serial.println(current);
        //   yield();
        // }
        for (; current - initial <= wait; current = millis()) {
            Serial.println(current);
            if (!digitalRead(downButtonPin) || !digitalRead(upButtonPin)) {
                // calculate percent and return
                int perCal = millis() - initial;
                if (up) {
                    perCal /= up1per;
                } else {
                    perCal /= down1per;
                }
                return perCal;
            }
            yield();
        }
        return 666;
    }

    int setDirForMil(bool up, long mil) {
        int ret;
        if (up) {
            digitalWrite(relayUpPin, LOW);
            ret = waitForMil(mil, up);
            digitalWrite(relayUpPin, HIGH);
        } else {
            digitalWrite(relayDownPin, LOW);
            ret = waitForMil(mil, up);
            digitalWrite(relayDownPin, HIGH);
        }
        return ret;
    }

    bool checkZero() {
        // if switch is on
        if (digitalRead(switchPin)) {
            unsigned long startClock = millis();
            // if its on top for 200 milliseconds
            while (1) {
                if (millis() - startClock >= 200) return true;
                if (!digitalRead(switchPin)) return false;
            }
        }
        return false;
    }

    int zeroLoop() {
        unsigned long initialTime = millis();
        // Going up
        digitalWrite(relayUpPin, LOW);

        while (true) {
            // delay(delayTimeout);
            // if it has reached zero with switch
            Serial.println(digitalRead(switchPin));
            if (checkZero()) {
                digitalWrite(relayUpPin, HIGH);
                blindsLevel = 0;
                return 0;
            }

            // if a button is pressed
            if (!digitalRead(downButtonPin) || !digitalRead(upButtonPin)) {
                digitalWrite(relayUpPin, HIGH);
                return 1;
            }

            // if it takes too long
            if (millis() - initialTime > timeOutTime) {
                digitalWrite(relayUpPin, HIGH);
                return 2;
            }
            yield();
        }
    }

    void handleZero() {
        if (!checkZero()) {
            Serial.println("Not on top entering zeroloop");
            unsigned long initialTime = millis();
            int res = zeroLoop();
            Serial.printf("Exited zeroloop with code:%d\n", res);
            // if button pressed
            if (res == 1) {
                int perCal = millis() - initialTime;
                perCal /= up1per;
                blindsLevel -= perCal;
            } else {
                blindsLevel = 0;
            }
        } else {
            blindsLevel = 0;
        }
        // sendAllValues();
    }

    void gotoPercent(int per) {
        int ok;
        int haha;
        // go down
        if (blindsLevel < per) {
            ok = down1per * (per - blindsLevel);
            Serial.printf("Going down for %d milis\n", ok);
            haha = setDirForMil(false, (long)ok);
            if (haha == 666) {
                blindsLevel = per;
            } else {
                blindsLevel += haha;
            }

        } else if (blindsLevel > per) {
            // go up
            // if its going to zero
            if (per == 0) {
                handleZero();
                return;
            }
            ok = up1per * (blindsLevel - per);
            Serial.printf("Going up for %d milis\n", ok);
            haha = setDirForMil(true, (long)ok);
            if (haha == 666) {
                blindsLevel = per;
            } else {
                blindsLevel -= haha;
            }
        }
        gotoLevel = -1;
        // sendAllValues();
    }

    void buttonsLoop() {
        if (!digitalRead(this->upButtonPin) && digitalRead(this->downButtonPin)) {
            Serial.println("Got button up");
            delay(100);
            if (!digitalRead(this->upButtonPin)) {
                delay(200);
                gotoPercent(0);
                delay(250);
            }
        } else if (!digitalRead(this->downButtonPin) && digitalRead(this->upButtonPin)) {
            Serial.println("Got button down");
            delay(100);
            if (!digitalRead(this->downButtonPin)) {
                delay(200);
                gotoPercent(100);
                delay(250);
            }
        }
    }

   public:
    BlindsFeature(Device& device, String name, uint8_t relayUpPin, uint8_t relayDownPin, uint8_t switchPin, uint8_t upButtonPin, uint8_t downButtonPin);
    ~BlindsFeature();

    void getData(const JsonObject& doc) {
        doc["blindsLevel"] = this->blindsLevel;
    }

    void execute(const JsonObjectConst& doc) {
        if (doc.containsKey("blindsLevel")) {
            gotoLevel = doc["blindsLevel"];
            // gotoPercent(doc["blindsLevel"]);
        }
    }

    void loop() {
        this->buttonsLoop();
        if (gotoLevel != -1) {
            gotoPercent(gotoLevel);
            gotoLevel = -1;
            delay(100);
        }
    }

    void setup() {
        this->handleZero();
    }
};

BlindsFeature::BlindsFeature(Device& device, String name, uint8_t relayUpPin, uint8_t relayDownPin, uint8_t switchPin, uint8_t upButtonPin, uint8_t downButtonPin) : Feature("BlindsFeature", device, name) {
    this->relayUpPin = relayUpPin;
    this->relayDownPin = relayDownPin;
    this->switchPin = switchPin;
    this->upButtonPin = upButtonPin;
    this->downButtonPin = downButtonPin;
}

BlindsFeature::~BlindsFeature() {
}

}  // namespace home

#endif