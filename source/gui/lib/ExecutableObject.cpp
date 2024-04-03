#include "ExecutableObject.h"
#include "ExecutableObjectImpl.h"

ExecutableObject::ExecutableObject(Object* parent)
    : Object(parent)
{
}

ExecutableObject::~ExecutableObject() {
    if (m_impl) delete m_impl;
}

void ExecutableObject::exec() {
    m_impl->exec();
}

void ExecutableObject::stop() {
    m_impl->stop();
}

//ExecutableObjectImpl* ExecutableObject::createImpl() {
//    return new ExecutableObjectImpl;
//}

ExecutableObjectImpl* ExecutableObject::getImpl() const {
    return m_impl;
}