// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_lengthtime.h"


//cObject(const cFrame&);
//
//cObject::cObject(const cVec4D_big& _relPos, cObject& _reference, cBlop* blopPtr, uint16_t blopNum)
//	: cObject(_relPos, &_reference, blopPtr, blopNum)
//{}
//
//cObject::cObject(const cVec4D_big& _relPos, cObject* _reference, cBlop* blopPtr, uint16_t blopNum)
//	: reference(_reference), relpos(_relPos), numblops(blopNum)
//{
//
//	if (blopNum > 65535) throw;
//	if (blops) throw;
//
//	void* tmpBlopPtrArray = malloc(sizeof(cBlop*) * blopNum);
//	if (!tmpBlopPtrArray) throw;
//	blops = (cBlop*)tmpBlopPtrArray;
//	memcpy(blops, blopPtr, sizeof(cBlop*) * blopNum);
//
//	if (reference) {
//		level = reference->level + 1;
//	}
//}
//
//cObject::~cObject() {
//	for (uint16_t i = 0; i != numblops; ++i) {
//		//			delete blops[i];
//	}
//	free(blops);
//}
//
//
//void cObject::refreshBoundingBox() {
//	_boundingMins.x = INFINITY;
//	_boundingMins.y = INFINITY;
//	_boundingMins.z = INFINITY;
//	_boundingMaxs.x = -INFINITY;
//	_boundingMaxs.y = -INFINITY;
//	_boundingMaxs.z = -INFINITY;
//	for (uint16_t i = 0; i != numblops; ++i) {
//		// Check all vertices, grab lowest values
//		blops[i].getBounding(_boundingMins, _boundingMaxs);
//	}
//}
//
//const cVec4D_dbl& cObject::boundingMins() const {
//	return _boundingMins;
//}
//
//const cVec4D_dbl& cObject::boundingMaxs() const {
//	return _boundingMaxs;
//}