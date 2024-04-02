// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/helpers/ntree.decl.h"

using cQuadtree = cNtree<2>;
using cOctree = cNtree<3>;
using cOctreeMember = cNtreeMember<3>;

template <uint8_t D>
cNtreeMember<D>::cNtreeMember(const Vec<D>& tmpPosition, cNtree<D>* tmpOwner)
	: position(tmpPosition), ntree(tmpOwner)
{
	if (ntree) ntree->add(this);
}

template <uint8_t D>
cNtreeMember<D>::~cNtreeMember() {
	if (ntree) ntree->remove(this);
}

template <uint8_t D>
Vec<D> cNtreeMember<D>::getPosition() const {
	return position;
}

template <uint8_t D>
void cNtreeMember<D>::setPosition(const Vec<D>& newPosition) {
	position = newPosition;
	if (ntant)	getOwnerNtree()->remove(this);
	getOwnerNtree()->add(this);
}

template <uint8_t D>
cNtant<D>::cNtant(cNtant<D>* const tmpParentPtr, const Vec<D>& lowBound, const Vec<D>& highBound)
	: parent(tmpParentPtr), boundingBox(lowBound, highBound)
{
	splitpoint = (lowBound + highBound) / 2.0;
	directMembers = new std::set<cNtreeMember<D>*>;
}

template <uint8_t D>
cNtant<D>::~cNtant()
{
	if (this->directMembers) {
		// Tell any direct members that they are no longer owned by this ntant
		for (typename std::set<cNtreeMember<D>*>::iterator it = this->directMembers->begin(); it != this->directMembers->end(); ++it) {
			(*it)->setNtant(0);
		}

		delete directMembers;
	}

	for (unsigned int i = 0; i != 8; ++i) {
		if (children[i]) {
			delete children[i];
			children[i] = 0;
		}
	}
}

template <uint8_t D>
void cNtant<D>::_add(cNtreeMember<D>* const newMember) {

	// Subdivide if too many direct members already
	if (!isSubdivided) {
		if (!directMembers) throwl("Ntree direct member vector not accessible");
		if (directMembers->size() == maxMembersBeforeSubdivision) _subdivide();
	}

	// If it is subdivided, add to subdivision, otherwise add as direct member
	if (isSubdivided) {
		Vec<D> newMemberPosition = newMember->getPosition();
		size_t i = 0;
		for (uint8_t dim = D; dim != 0; --dim) {
			if (newMemberPosition.dim(dim) > splitpoint.dim(dim)) i += pwrtwo(dim);
		}
		//if (newMemberPosition.x() > splitpoint.x()) i += 4;
		//if (newMemberPosition.y() > splitpoint.y()) i += 2;
		//if (newMemberPosition.z() > splitpoint.z()) i += 1;
		children[i]->_add(newMember);
		_indirectMemberAdded();
	}
	else {
		if (!directMembers) throwl("Ntree direct member vector not available");
		directMembers->insert(newMember);
		newMember->setNtant(this);
	}
}

template <uint8_t D>
void cNtant<D>::_subdivide() {
	if (isSubdivided) throwl("Already subdivided");

	Vec<D> lowBound = boundingBox.getLowerBound();
	Vec<D> highBound = boundingBox.getLowerBound();
	Vec<D> exteriorBound;
	for (size_t n = 0; D != pwrtwo(D); ++n) {
		// for each child ntant, need to build up correct exterior vector
		for (uint8_t dim = 0; dim != D; dim) {
			const size_t dimpow = pwrtwo(dim);
			if (n && dimpow) { // high
				exteriorBound.dim(D - dim) = highBound.dim(dim);
			}
			else { // low
				exteriorBound.dim(D - dim) = lowBound.dim(dim);
			}
		}
		children[n] = new cNtant(this, exteriorBound, splitpoint);
	}

	//children[0] = new cNtant(this, lowBound, splitpoint);
	//children[1] = new cNtant(this, { lowBound.x(), lowBound.y(), highBound.z() }, splitpoint);
	//children[2] = new cNtant(this, { lowBound.x(), highBound.y(), lowBound.z() }, splitpoint);
	//children[3] = new cNtant(this, { lowBound.x(), highBound.y(), highBound.z() }, splitpoint);
	//children[4] = new cNtant(this, { highBound.x(), lowBound.y(), lowBound.z() }, splitpoint);
	//children[5] = new cNtant(this, { highBound.x(), lowBound.y(), highBound.z() }, splitpoint);
	//children[6] = new cNtant(this, { highBound.x(), highBound.y(), lowBound.z() }, splitpoint);
	//children[7] = new cNtant(this, splitpoint, highBound);

	isSubdivided = true;

	// Re-add direct members to subdivisions
	std::set<cNtreeMember<D>*>* oldMemberVectorPtr = directMembers;
	directMembers = 0;

	if (oldMemberVectorPtr) {
		// Tell any direct members that they are no longer owned by this ntant, move to childern
		for (typename std::set<cNtreeMember<D>*>::iterator it = oldMemberVectorPtr->begin(); it != oldMemberVectorPtr->end(); ++it) {
			_add(*it);
		}

		delete oldMemberVectorPtr;
	}
}

template <uint8_t D>
void cNtant<D>::_remove(cNtreeMember<D>* const memberToRemove) {
	// If not subdivided, try to remove directly
	if (!isSubdivided) {
		if (!directMembers) throwl("Could not access direct member set");
		if (directMembers->count(memberToRemove)) {
			directMembers->erase(memberToRemove);
			memberToRemove->setNtant(0);
			if (parent) parent->_indirectMemberRemoved();
		}
		else {
			throwl("Member not found");
		}
	}
	else {
		throwl("Tried to remove direct member from subdivided ntant");
	}
}

template <uint8_t D>
cNtree<D>::cNtree(const Vec<D>& lowerBound, const Vec<D>& upperBound)
	: cNtant<D>(0, lowerBound, upperBound)
{}

template <uint8_t D>
void cNtree<D>::add(cNtreeMember<D>* const newMember) {
	//Check if within bounds
	if (!this->boundingBox.contains(newMember->getPosition())) {
		throwl("New member is outside bounds of ntree");
	}
	this->_add(newMember);
}

template <uint8_t D>
void cNtree<D>::remove(cNtreeMember<D>* const memberToRemove) {
	if (!memberToRemove) throwl("Trying to remove non-existent member");
	try {
		cNtant<D>* containingNtant = memberToRemove->getNtant();
		containingNtant->_remove(memberToRemove);
		memberToRemove->setNtant(0);
	}
	// If you are so inclined, you could implement a back-up search for the member here based on its location
	catch_rethrow("Could not remove member from supposed containing ntant (containing ntant pointer not properly updated?");	
}

template <uint8_t D>
void cNtant<D>::_indirectMemberRemoved() {
	--indirectMemberCount;
	if (indirectMemberCount < minimumMembersBeforeCollapse) _collapse();
}

template <uint8_t D>
void cNtant<D>::_indirectMemberAdded() {
	++indirectMemberCount;
}

template <uint8_t D>
void cNtant<D>::_collapse() {
	std::cout << "Should collapse ntant here\n";

	if (!isSubdivided) return; // Nothing to do, already collapsed

	// Declare not subdivided
	isSubdivided = false;

	// Promote members from each child and then delete child

	for (unsigned int i = 0; i != 8; ++i) {
		if (children[i]) {
			children[i]->_promoteMembers();
			delete children[i];
			children[i] = 0;
		}
	}
	indirectMemberCount = 0;
}

template <uint8_t D>
void cNtant<D>::_promoteMembers() {
	if (this->directMembers) {
		// Tell any direct members that they are no longer owned by this ntant, add them to parent
		for (typename std::set<cNtreeMember<D>*>::iterator it = this->directMembers->begin(); it != this->directMembers->end(); ++it) {
			(*it)->setNtant(0);
			parent->_add(*it);
		}
		delete this->directMembers;
		this->directMembers = 0;
	}
}