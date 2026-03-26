#include "EventBus.h"

Event EventBus::queue[10];
int EventBus::head = 0;
int EventBus::tail = 0;

void EventBus::push(Event e) {
    queue[tail] = e;
    tail = (tail + 1) % 10;
}

bool EventBus::hasEvents() {
    return head != tail;
}

Event EventBus::poll() {
    if (!hasEvents()) return { EVENT_NONE, 0 };
    Event e = queue[head];
    head = (head + 1) % 10;
    return e;
}