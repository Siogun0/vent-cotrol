#ifndef EEZ_LVGL_UI_STRUCTS_H
#define EEZ_LVGL_UI_STRUCTS_H

#include "eez-flow.h"

#include <stdint.h>
#include <stdbool.h>

#include "vars.h"

using namespace eez;

enum FlowStructures {
    FLOW_STRUCTURE_ROOM_STATE = 16384
};

enum FlowArrayOfStructures {
    FLOW_ARRAY_OF_STRUCTURE_ROOM_STATE = 81920
};

enum room_stateFlowStructureFields {
    FLOW_STRUCTURE_ROOM_STATE_FIELD_TEMPERATURE = 0,
    FLOW_STRUCTURE_ROOM_STATE_FIELD_HUMIDITY = 1,
    FLOW_STRUCTURE_ROOM_STATE_FIELD_AIR_QUOLITY = 2,
    FLOW_STRUCTURE_ROOM_STATE_FIELD_TVOC = 3,
    FLOW_STRUCTURE_ROOM_STATE_FIELD_E_CO2 = 4,
    FLOW_STRUCTURE_ROOM_STATE_FIELD_PRESENCE = 5,
    FLOW_STRUCTURE_ROOM_STATE_NUM_FIELDS
};

struct room_stateValue {
    Value value;
    
    room_stateValue() {
        value = Value::makeArrayRef(FLOW_STRUCTURE_ROOM_STATE_NUM_FIELDS, FLOW_STRUCTURE_ROOM_STATE, 0);
    }
    
    room_stateValue(Value value) : value(value) {}
    
    operator Value() const { return value; }
    
    operator bool() const { return value.isArray(); }
    
    float temperature() {
        return value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_TEMPERATURE].getFloat();
    }
    void temperature(float temperature) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_TEMPERATURE] = FloatValue(temperature);
    }
    
    float humidity() {
        return value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_HUMIDITY].getFloat();
    }
    void humidity(float humidity) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_HUMIDITY] = FloatValue(humidity);
    }
    
    int air_quolity() {
        return value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_AIR_QUOLITY].getInt();
    }
    void air_quolity(int air_quolity) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_AIR_QUOLITY] = IntegerValue(air_quolity);
    }
    
    int TVOC() {
        return value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_TVOC].getInt();
    }
    void TVOC(int TVOC) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_TVOC] = IntegerValue(TVOC);
    }
    
    int eCO2() {
        return value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_E_CO2].getInt();
    }
    void eCO2(int eCO2) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_E_CO2] = IntegerValue(eCO2);
    }
    
    human_presence presence() {
        return (human_presence)value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_PRESENCE].getInt();
    }
    void presence(human_presence presence) {
        value.getArray()->values[FLOW_STRUCTURE_ROOM_STATE_FIELD_PRESENCE] = IntegerValue((int)presence);
    }
};

typedef ArrayOf<room_stateValue, FLOW_ARRAY_OF_STRUCTURE_ROOM_STATE> ArrayOfroom_stateValue;

#endif /*EEZ_LVGL_UI_STRUCTS_H*/