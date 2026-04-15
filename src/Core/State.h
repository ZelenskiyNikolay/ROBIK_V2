#pragma once
#include "EventBus.h"

enum StateID {
    STATE_START = 0,
    STATE_CALIBRATION = 1,
    STATE_NORMAL = 2,
    STATE_SLEEPY = 3,
    STATE_CLOCK  = 4,
    STATE_SEARCH_BASE = 5,
    STATE_USB = 6,
    STATE_VOICE_RECORD = 7,
    STATE_IDEL = 8
};

class State {
public:
    virtual ~State() {} 

    virtual void enter() {}
    virtual void update(float dt) {}
    virtual void exit() {};
};
