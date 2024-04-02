// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector_3d.h"
#include "Utilogeny/source/physics/core/physics/helpers/ntree.h"



//
//class cOctant;
//class cOctree;
//
//class cOctreeMember {
//public:
//	cOctreeMember(const Vec3D &, cOctree *);
//	~cOctreeMember();
//	Vec3D getPosition() const;
//	void setPosition(const Vec3D& newPosition);
//	cOctree* getOwnerOctree() const { return ntree; };
//private:
//	friend class cOctant;
//	friend class cOctree;
//	void setOctant(cOctant* const newOctant) { ntant = newOctant; }
//	cOctant* getOctant() const { return ntant; }
//	cOctant* ntant = 0;
//	cOctree* ntree = 0;
//	Vec3D position;
//};
//
//class cOctant {
//public:
//	cOctant(cOctant* const, const Vec3D&, const Vec3D&);
//	~cOctant();
//protected:
//	friend class cOctree;
//	void _add(cOctreeMember* const);
//	void _subdivide();
//	void _remove(cOctreeMember* const);
//	void _indirectMemberAdded();
//	void _indirectMemberRemoved();
//	void _collapse();
//	void _promoteMembers();
//	cOctant* parent = 0;
//	Vec3D splitpoint;
//	cBoundingBox3D boundingBox;
//	bool isSubdivided = false;
//	cOctant* children[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//	std::set<cOctreeMember*> * directMembers;
//	uint64_t indirectMemberCount = 0;
//	uint64_t maxMembersBeforeSubdivision = 24;
//	uint64_t minimumMembersBeforeCollapse = 8;
//};
//
//class cOctree : public cOctant {
//public:
//	cOctree(const Vec3D&, const Vec3D&);
//	void add(cOctreeMember* const);
//	void remove(cOctreeMember* const);
//};