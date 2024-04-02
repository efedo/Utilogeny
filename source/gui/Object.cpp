#include "Object.h"
#include "Application.h"

Object::Object(Object* parent) : m_parent(parent) {
    if (m_parent == 0) m_parent = &app;
}
