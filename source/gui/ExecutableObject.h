#pragma once

#include "Object.h"

class ExecutableObjectImpl;

class ExecutableObject : public Object {
public:
	ExecutableObject(Object* parent = 0);
	~ExecutableObject();
	void exec();
	void stop();
protected:
	friend class SDLWindowImpl;
	ExecutableObjectImpl * m_impl = 0;
	ExecutableObjectImpl * getImpl() const;
};
