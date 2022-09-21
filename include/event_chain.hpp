#pragma once

struct EventChain {
    EventChain();

    Event* head;
    int difficulty;

    void mine(Event*);
};
