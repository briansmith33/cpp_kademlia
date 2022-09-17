#include "../include/event.hpp"

Event::Event() {}

Event::Event(std::string d, std::byte s) {
    data = d;
    signature = s;
}
