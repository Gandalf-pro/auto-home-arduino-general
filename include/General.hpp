#ifndef general__1
#define general__1

#include <Arduino.h>
#include <ArduinoJson.h>

#define N 10

namespace home {
class Feature;
class Device;

class Feature {
   private:
    /* data */
    Device* device;
    String name;
    String type;

   public:
    String availableLogics[2] = {"gfd", "asd"};
    String availableActions[2] = {"gfd", "asd"};
    Feature(Device& device, String name);
    ~Feature();

    String getName() {
        return this->name;
    }

    virtual void execute(const ArduinoJson6192_F1::VariantConstRef& doc) = 0;

    virtual String getData() = 0;

    virtual String toJson() = 0;

    virtual void loop(){};
};

Feature::Feature(Device& device, String name) {
    this->device = &device;
    this->name = name;
}

Feature::~Feature() {
}

class Device {
   private:
    /* data */
    String room;
    String name;

    Feature* features[N];
    size_t featuresLength = 0;

    void getFeaturesArray(JsonArray& arr) {
        for (size_t i = 0; i < this->featuresLength; i++) {
            // JsonObject obj =  arr.createNestedObject();
            arr.add(serialized(this->features[i]->toJson()));
        }
    }

   public:
    Device(String room, String name);
    ~Device();

    String getUrl() {
        return this->room + "/" + this->name;
    }

    String getName() {
        return this->name;
    }

    void setName(String name) {
        this->name = name;
    }

    String getRoom() {
        return this->room;
    }

    void setRoom(String room) {
        this->room = room;
    }

    void addFeature(Feature& feature) {
        if (this->featuresLength >= N) {
            return;
        }
        this->features[this->featuresLength] = &feature;
        this->featuresLength++;
    }

    Feature* getFeature(String name) {
        size_t count = this->featuresLength;
        for (size_t i = 0; i < count; i++) {
            if (this->features[i]->getName() == name) {
                return this->features[i];
            }
        }
        return nullptr;
    }

    Feature** getFeatures() {
        return this->features;
    }

    int getFeaturesLength() {
        return this->featuresLength;
    }

    String getData() {
        StaticJsonDocument<256> doc;
        JsonArray featuresArray = doc.createNestedArray("features");
        this->getFeaturesArray(featuresArray);
        doc["name"] = this->getName();
        doc["room"] = this->getRoom();
        String output;
        serializeJson(doc, output);
        return output;
    }

    void loop() {
        // Loop for features
        for (size_t i = 0; i < this->featuresLength; i++) {
            this->features[i]->loop();
        }
    }

    void execute(DynamicJsonDocument const& doc) {
        // Loop through features to see if key belongs to it
        for (size_t i = 0; i < this->featuresLength; i++) {
            Feature* fea = this->features[i];
            if (doc.containsKey("feature") && strcmp(doc["feature"], fea->getName().c_str()) == 0) {
                // Execute that features execute funtion
                // auto const& haha = doc[""];
                // auto papa = doc[""];
                Serial.println("Executing feature:" + fea->getName());
                fea->execute(doc["data"]);
            }
        }
    }
};

Device::Device(String room, String name) {
    this->room = room;
    this->name = name;
}

Device::~Device() {
}

}  // namespace home

#endif