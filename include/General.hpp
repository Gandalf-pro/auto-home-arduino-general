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
    Feature(String type, Device& device, String name);
    ~Feature();

    String getName() {
        return this->name;
    }

    String getType() {
        return this->type;
    }

    virtual void execute(const JsonObjectConst& doc) = 0;

    virtual void getData(const JsonObject& doc) = 0;

    virtual String toJson() {
        StaticJsonDocument<360> doc;
        doc["name"] = this->getName();
        doc["type"] = this->getType();
        auto data = doc.createNestedObject("data");

        // Data
        this->getData(data);

        String output;
        serializeJson(doc, output);
        return output;
    };

    virtual void loop(){};
    virtual void setup(){};
};

Feature::Feature(String type, Device& device, String name) {
    this->type = type;
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

    void getFeaturesArray(const JsonArray& arr) {
        for (size_t i = 0; i < this->featuresLength; i++) {
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

    void addFeature(Feature* feature) {
        if (this->featuresLength >= N) {
            return;
        }
        this->features[this->featuresLength] = feature;
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
        DynamicJsonDocument doc(1024);
        JsonArray featuresArray = doc.createNestedArray("features");
        this->getFeaturesArray(featuresArray);
        doc["name"] = this->getName();
        doc["room"] = this->getRoom();
        String output;
        if(doc.overflowed()){
            return "Overflowed";
        }
        serializeJson(doc, output);
        return output;
    }

    void loop() {
        // Loop for features
        for (size_t i = 0; i < this->featuresLength; i++) {
            this->features[i]->loop();
        }
    }
    void setup() {
        // Loop for setups
        for (size_t i = 0; i < this->featuresLength; i++) {
            this->features[i]->setup();
        }
    }

    void execute(const DynamicJsonDocument& doc) {
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