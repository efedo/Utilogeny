// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/universe/generators.h"
#include "Utilogeny/source/physics/lib/universe/universe.h"
#include "Utilogeny/source/physics/lib/forces/forces.h"
#include "Utilogeny/source/physics/lib/objects/origin.h"
#include "Utilogeny/source/physics/lib/objects/particleclass.h"
#include "Utilogeny/source/physics/lib/objects/particle.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"
#include "Utilogeny/source/physics/lib/vector/special/vector_length.h"
#include "Utilogeny/source/physics/lib/forces/gravity.h"
#include "Utilogeny/source/physics/lib/forces/property.h"

// Just playing around, this does not represent anything real
cUniverse* UniverseGenerator::createTernaryPhotonSystem() {

	cUniverse* uniptr = new cUniverse;
	cUniverse& uni = *uniptr;

	cProperty* propPhotonA = uni._addProperty("Photon A property");
	cProperty* propPhotonB = uni._addProperty("Photon B property");

	uni._addForce(propPhotonA, propPhotonB, 0, -7.07E-13, 2); // Photon particle A attracts particle B
	uni._addForce(propPhotonB, propPhotonA, 0, 1.59E-12, 3); // Photon particle B repels particle A

	cParticleClass* class_photonA = uni._addParticleClass("Photon particle A");
	class_photonA->setProperty(propPhotonA, 1.0);

	cParticleClass* class_photonB = uni._addParticleClass("Photon particle B");
	class_photonB->setProperty(propPhotonB, 1.0);

	auto pA = uni._addParticle(class_photonA);
	pA->setPosition(Vec3D(0, 1e-11, 0));

	auto pB1 = uni._addParticle(class_photonB);
	pB1->setPosition(Vec3D(1e-30, 0, 0));

	auto pB2 = uni._addParticle(class_photonB);
	pB2->setPosition(Vec3D(-1e-30, 0, 0));

	//std::ofstream physicsfile("physicsout.tsv", std::ofstream::trunc);

	//if (!physicsfile.good()) {
	//	std::cerr << "Could not open physics file.\n";
	//	return;
	//}

	//physicsfile << "t\tdB:B\tdA:Bave\tekA\tvA(m/s)\tvA(c)\n";

	//const unsigned int imax = 10000;
	//for (unsigned int i = 0; i != imax; ++i) {
	//	physicsfile << std::fixed << std::setprecision(20) << time << "\t";
	//	physicsfile << std::scientific << std::setprecision(3) << (pB1->getPosition() - pB2->getPosition()).magnitude() << "\t";
	//	physicsfile << std::scientific << std::setprecision(3) << (pA->getPosition() - 0.5 * (pB1->getPosition() + pB1->getPosition())).magnitude() << "\t";
	//	physicsfile << std::scientific << std::setprecision(3) << pA->getKinEnergy().magnitude() << "\t";
	//	physicsfile << std::scientific << std::setprecision(3) << pA->getVelocity().magnitude() << "\t";
	//	physicsfile << std::scientific << std::setprecision(10) << pA->getVelocity().magnitude() / _maxVelocity << "\n";
	//	
	//	if ((i < 1000) || (!(i % 1000))) {
	//		std::cout << "\rCompleted physics iteration " << i << " out of " << imax;
	//	}
	//	_calcImpulses();
	//	_advanceTime();
	//}
	std::cout << "\n";


	return uniptr;
}