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
    String type = "AcFeature";

   public:
    AcFeature(Device& device, String name, uint8_t pin);
    ~AcFeature();

    String getData() {
        if (this->ac == nullptr) {
            return "{}";
        }
        String message = "{";
        message += "\"power\":" + String(ac->getPower());
        message += ", \"temp\":" + String(ac->getTemp());
        message += ", \"fan\":" + String(ac->getFan());
        message += ", \"mode\":" + String(ac->getMode());
        message += ", \"swing\":" + String(ac->getSwing());
        return message + "}";
    }

    String toJson(){
        String message = "{";
        message += "\"name\":" + this->getName();
        message += ",\"type\":" + this->type;
        message += ",\"data\":" + this->getData();
        return message + "}";
    }

    void sendAcSignal() {
        ac->send(6);
    }

    void execute(const ArduinoJson6192_F1::VariantConstRef& doc) {
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
};

AcFeature::AcFeature(Device& device, String name, uint8_t pin) : Feature(device, name) {
    ac = new IRVestelAc(pin);
}

AcFeature::~AcFeature() {
}

}  // namespace home

#endif