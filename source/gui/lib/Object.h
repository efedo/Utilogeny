#pragma once

class Object;

class Object {
public:
	Object(Object* parent);
private:
	Object* m_parent = 0;
};
