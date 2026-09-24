#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
  uint8_t name[30];
  uint32_t supply_fan_speed;
  uint32_t exhaust_fan_speed;
  uint32_t family_room_valve;
  uint32_t master_badroom_valve;
  uint32_t children_room_valve;
  uint32_t pantry_room_valve;
  uint32_t toilet_exhaust_valve;
  uint32_t bathroom_exhaust_valve;
} scenario_t;

extern scenario_t scenarios[];
extern const size_t scenarios_number;

#define MANUAL_CONTROL 1
#define AUTOMATIC_CONTROL 0
#endif //SCENARIOS_H