#include "Application.h"
#include "ExecutableObjectImpl.h"
#include "ExtendedException.h"

Doodad::Doodad() {
    m_impl = new ExecutableObjectImpl;
    m_impl->exec();
}

Doodad app;