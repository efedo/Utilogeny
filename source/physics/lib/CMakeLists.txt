cmake_minimum_required(VERSION 3.25)

add_library(UtilogenyPhysics STATIC
    interface/object_interface.cpp
    interface/object_interface.h
    interface/universe_interface.cpp
    interface/universe_interface.h
    forces/forces.cpp
    forces/forces.h
    forces/gravity.cpp
    forces/gravity.h
    forces/property.cpp
    forces/property.h
    helpers/bounding.cpp
    helpers/bounding.h
    helpers/ntree.decl.h
    helpers/ntree.h
    helpers/octree.h
    objects/atom.h
    objects/frame.cpp
    objects/frame.h
    objects/object.cpp
    objects/object.h
    objects/object_compound.cpp
    objects/object_compound.h
    objects/origin.h
    objects/particle.cpp
    objects/particle.h
    objects/particleclass.cpp
    objects/particleclass.h
    relativity/relativity.h
    universe/universe.cpp
    universe/universe.h
    universe/generators.h
    universe/generators/standard_atomic.cpp
    universe/generators/funny_photon_system.cpp
    vector/special/vector_length.h
    vector/special/vector_lengthtime.h
    vector/special/vector_unit3d.h
    vector/special/vector_velocity.h
    vector/vector.h
    precomp.cpp
    unitlib/helpers/constants_private.h
    unitlib/helpers/dimension_fundamentals.h
    unitlib/helpers/dimension_templates.h
    unitlib/helpers/helper_macros.h
    unitlib/helpers/macro_debugging.h
    unitlib/helpers/si_prefixes.h
    unitlib/units.cpp
    unitlib/units.h
    unitlib/constants.h
    unitlib/units/acceleration.h
    unitlib/units/amount.h
    unitlib/units/angle.h
    unitlib/units/area.h
    unitlib/units/base.h
    unitlib/units/charge.h
    unitlib/units/concentration.h
    unitlib/units/cycle.h
    unitlib/units/energy.cpp
    unitlib/units/energy.h
    unitlib/units/frequency.h
    unitlib/units/length.h
    unitlib/units/mass.cpp
    unitlib/units/mass.h
    unitlib/units/temperature.h
    unitlib/units/time.h
    unitlib/units/velocity.h
    unitlib/units/volume.h
    unitlib/units/pressure.h)

target_precompile_headers(UtilogenyPhysics
	PRIVATE precomp.h)
	
target_config_compiler(UtilogenyPhysics)

set_target_properties(UtilogenyPhysics PROPERTIES
           CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
           CXX_EXTENSIONS OFF
           )