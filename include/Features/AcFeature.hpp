#ifndef AcFeature__
#define AcFeature__

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Vestel.h>

#include <General.hpp>

namespace home {
class AcFeature : public Feature {
   private:
    /* data */
    IRVestelAc* ac = nullptr;

   public:
    AcFeature(Device& device, String name, uint8_t pin);
    ~AcFeature();

    void getData(const JsonObject& doc) {
        if (this->ac == nullptr) {
            return;
        }
        doc["power"] = String(ac->getPower());
        doc["temp"] = String(ac->getTemp());
        doc["fan"] = String(ac->getFan());
        doc["mode"] = String(ac->getMode());
        doc["swing"] = String(ac->getSwing());
    }

    void sendAcSignal() {
        ac->send(6);
    }

    void execute(const JsonObjectConst& doc) {
        if (this->ac == nullptr) {
            return;
        }
        bool changed = false;
        if (doc.containsKey("power")) {
            ac->setPower(doc["power"]);
            changed = true;
        }
        if (doc.containsKey("temp")) {
            ac->setTemp(doc["temp"]);
            changed = true;
        }

        if (doc.containsKey("fan")) {
            int mod = doc["fan"];
            switch (mod) {
                case 0:
                    ac->setFan(kVestelAcFanAuto);
                    break;
                case 1:
                    ac->setFan(kVestelAcFanLow);
                    break;
                case 2:
                    ac->setFan(kVestelAcFanMed);
                    break;
                case 3:
                    ac->setFan(kVestelAcFanHigh);
                    break;

                default:
                    break;
            }
            if (mod >= 0 && mod <= 3) {
                changed = true;
            }
        }
        if (doc.containsKey("mode")) {
            ac->setMode(doc["mode"]);
            changed = true;
        }
        if (doc.containsKey("swing")) {
            ac->setSwing(doc["swing"]);
            changed = true;
        }

        if (changed) {
            sendAcSignal();
        }
    }

    void setup() {
        ac->begin();
        delay(200);
        ac->off();
        ac->setTemp(26);
        ac->setFan(kVestelAcFanMed);
        ac->setMode(kVestelAcCool);
    }
};

AcFeature::AcFeature(Device& device, String name, uint8_t pin) : Feature("AcFeature", device, name) {
    ac = new IRVestelAc(pin);
}

AcFeature::~AcFeature() {
}

}  // namespace home

#endif