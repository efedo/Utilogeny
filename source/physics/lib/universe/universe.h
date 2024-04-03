// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/lib/vector/special/vector_length.h"
#include "Utilogeny/source/physics/lib/unitlib/units/length.h"

class cOrigin;
class cForce;
class cProperty;
class cParticle;
class cParticleClass;
class cGravityTensor;

class cUniverse {
public:
	uint8_t						getDimensions() const;
	cOrigin*					getOrigin() const;
	double						getMaxVelocity() const;
	cGravityTensor				getGravity(const VecLength3 &) const; /// Gets gravity field and frame at the given
														// position
	double						getTickLength() const;
	void						addProperty(const std::string &);
	void						addForce(const std::string &, const std::string &, const std::string&, const double &, const unsigned int &);
	void						_updatePClassForceListsAll();
	void						_advanceTime();
	cUniverse();
	~cUniverse();
private:
	friend class UniverseGenerator;
	void						_calcImpulses();
	cProperty *					_addProperty(const std::string &);
	cForce *					_addForce(const std::string &, const std::string &, const std::string &, const double &, const unsigned int &);
	cForce *					_addForce(cProperty *, cProperty *, cProperty *, const double &, const unsigned int &);
	cParticleClass *			_addParticleClass(const std::string &);
	cParticle *					_addParticle(const std::string &);
	cParticle *					_addParticle(cParticleClass *);
	cProperty *					_getProperty(const std::string &) const;
	cForce *					_getForce(const std::string &, const std::string &) const;
	cForce *					_getForce(cProperty *, cProperty *) const;
	cParticleClass *			_getParticleClass(const std::string &) const;
	void						_updatePClassForceLists(cForce *);

	uint8_t						_dimensions = 3;
	cOrigin* const				_origin = 0;
	double						_maxVelocity = 299792458; // ms-1
	double						ticklength = 1e-15; // s
	double						time = 0;
	std::map<std::string,
		cProperty*>				properties;
	std::map<cProperty*,
		std::map<cProperty*,
		cForce*>>				forces;
	std::map<std::string,
		cParticleClass*>		pclasses;
	std::list<cParticle*>		particles;
};