#pragma once
#include <Arduino.h>

enum EventType {
  EVENT_NONE,
  EVENT_CHANGE_STATE,
};

struct Event {
  EventType type;
  uint16_t data;
};

class EventBus {
public:
    static void push(Event e);
    static Event poll();
    static bool hasEvents();

private:
    static Event queue[10];
    static int head;
    static int tail;
};