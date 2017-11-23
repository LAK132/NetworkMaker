#include "layoutprop.hpp"

Layout::Layout() {
}

Layout::~Layout() {
}

void Layout::addChild(Prop* p) {
    prop.push_back(p);
}

void Layout::clear() {
    prop.clear();
}

void VerticalLayout::draw(RenderTarget& target, RenderStates states) const {
    if (prop.size() <= 0) return;
    states.transform *= getTransform();
    for(auto it = prop.begin(); it != prop.end(); it++)
    {
        (*it)->size = Vector2f(size.x, size.y / prop.size());
        (*it)->draw(target, states);
        states.transform.translate(0, size.y / prop.size());
    }
}

void HorizontalLayout::draw(RenderTarget& target, RenderStates states) const {
    if (prop.size() <= 0) return;
    states *= getTransform();
    for(auto it = prop.begin(); it != prop.end(); it++)
    {
        (*it)->size = Vector2f(size.x / prop.size(), size.y);
        (*it)->draw(target, state);
        states.translate(size.x / prop.size(), 0);
    }
}
