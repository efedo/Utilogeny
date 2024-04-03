#include "RendererTypes.h"
#include "../ExtendedException.h"

RendererType m_RendererType = RendererType::any;
bool m_LockedRendererType = false;

RendererType getRendererType() {
    return m_RendererType;
}

void setRendererType(RendererType rendererType) {
    if (m_LockedRendererType) THROW_EXTENDED_EXCEPTION("Tried to reset the Doodad renderer system");
    m_RendererType = rendererType;
    m_LockedRendererType = true;
}

void lockRendererType() {
    m_LockedRendererType = true;
}