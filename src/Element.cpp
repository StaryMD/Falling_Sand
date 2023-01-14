#include "Element.hpp"

Element::Element() {
    flow_direction = 1;
}

Element::Element(const Substance substance) : Element() {
    this->substance = substance;
}

void Element::reverse_flow() {
    flow_direction *= -1;
}
