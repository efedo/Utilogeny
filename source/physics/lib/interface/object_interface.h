#pragma once

class cObject;

class cObjectInterface {
public:
	cObjectInterface(cObject*);
	void getPosition(double&, double&, double&);
	void getVelocity(double&, double&, double&);
	void getVelocityMagnitudeMPS(double &);
	void getVelocityMagnitudeC(double&);
private:
	cObject* _object = 0;
};