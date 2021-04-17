# Copyright 2021 Eric Fedosejevs
#

# CMake static type library
#
# ***INCOMPLETE***
#
# Synopsis:
#
# declare a bool:
# _bool(yourbool false)
#
# fails (not a bool):
# _bool(yourbool "crazyvalue")
#
# change value:
# _set(yourbool true)
#
# other value types:
# _int(yourint 5)
# _string(yourstring "hello I am a string")
# _list(yourboollist bool)
# _list(yourintlist int)
# _list(yourstringlist string)
# _list(yourmixedlist mixed)


# If implicit casts are not allowed, adds a layer of indirection to prevent variable use
option(CppMake_allow_implicit_casts OFF "Allows CppMake variables to be implicitly cast back to plain CMake variables")

macro(_CppMake_val_could_be value)
	unset(_CppMake_could_be_bool_flag)
	unset(_CppMake_could_be_int_flag)
	unset(_CppMake_could_be_string_flag)
	if(("${value}" STREQUAL "true") OR ("${value}" STREQUAL "false"))
		set(_CppMake_could_be_bool_flag)
	endif()
	
	unset(_CppMake_notintmatch)
	string(REGEX MATCH "[^-1234567890]" _CppMake_notintmatch "${value}")  

	if(NOT _CppMake_notintmatch)
		set(_CppMake_could_be_int_flag "true")
	endif()

	#message("_CppMake_notintmatch: ${_CppMake_notintmatch}")
	#message("_CppMake_could_be_int_flag: ${_CppMake_could_be_int_flag}")
	
	set(_CppMake_could_be_string_flag) # always can be string
endmacro()

macro(_CppMake_check_var_exists name)
	if(CppMake_allow_implicit_casts)
		if(NOT DEFINED "${name}")
			message("variable ${name}: ${${name}}")
			message(FATAL_ERROR "variable ${name} not declared")
		endif()
	endif()
	
	if(NOT _CppMake_vars_${name}_type)
		message(FATAL_ERROR "variable ${name} is not a CppMake variable")
	endif()
endmacro()

macro(_CppMake_check_var_available name)
	if("${name}")
		message(FATAL_ERROR "variable ${name} already declared")
	endif()
	if(TARGET "${name}")
		message(FATAL_ERROR "variable ${name} conflicts with target name")
	endif()
endmacro()

macro(_CppMake_set name value isnew)

	if(NOT ${isnew})
		_CppMake_check_var_exists(${name})
		if(CppMake_allow_implicit_casts)
			if(NOT ("${_CppMake_vars_${name}_value}" STREQUAL "${${name}}"))
				message(FATAL_ERROR "variable ${name} has been changed outside CppMake (value should have been \"${_CppMake_vars_${name}_value}\" but was \"${${name}}\")")
			endif()
		else()
			if(NOT ("${${name}}" STREQUAL "${name}-NOTFOUND"))
				message(FATAL_ERROR "variable ${name} has been changed outside CppMake (value should have been \"${name}-NOTFOUND\" but was \"${${name}}\")")
			endif()
		endif()
	endif()
	
	_CppMake_val_could_be("${value}")
	
	if(${_CppMake_vars_${name}_type} STREQUAL "bool")
		if("${value}" STREQUAL "true")
			if(CppMake_allow_implicit_casts)
				set(${name} ON)
			else()
				set(${name} "${name}-NOTFOUND")
			endif()
			set(_CppMake_vars_${name}_value "ON")
		elseif("${value}" STREQUAL "false")
			if(CppMake_allow_implicit_casts)
				set(${name} OFF)
			else()
				set(${name} "${name}-NOTFOUND")
			endif()
			set(_CppMake_vars_${name}_value "OFF")
		else()
			message(FATAL_ERROR "value ${value} is not a bool")
		endif()
	elseif(${_CppMake_vars_${name}_type} STREQUAL "int")
		#message("Should have set an int to ${value}!")
		if(_CppMake_could_be_int_flag)
			if(CppMake_allow_implicit_casts)
				set(${name} "${value}")
			else()
				set(${name} "${name}-NOTFOUND")
			endif()
			set(_CppMake_vars_${name}_value "${value}")
		else()
			message(FATAL_ERROR "value ${value} is not an int")
		endif()
	elseif(${_CppMake_vars_${name}_type} STREQUAL "string")
		if(_CppMake_could_be_string_flag)
			if(CppMake_allow_implicit_casts)
				set(${name} "${value}")
			else()
				set(${name} "${name}-NOTFOUND")
			endif()
			set(_CppMake_vars_${name}_value "${value}")
		else()
			message(FATAL_ERROR "value ${value} is not a string")
		endif()
	elseif(${_CppMake_vars_${name}_type} STREQUAL "list")
		message(FATAL_ERROR "list assignments not yet handled")
	else()
		message(FATAL_ERROR "value ${value} is not a valid CppMake variable type")
	endif()
	
	message("Set ${name} to ${_CppMake_vars_${name}_value}")
endmacro()

macro(_set name value)
	_CppMake_set(${name} "${value}" OFF)
endmacro()

# Declare a bool
macro(_bool name value)
	_CppMake_check_var_available(${name})
	set(_CppMake_vars_${name}_type "bool")
	_CppMake_set(${name} "${value}" ON)
endmacro()

# Declare an int
macro(_int name value)
	_CppMake_check_var_available(${name})
	set(_CppMake_vars_${name}_type "int")
	_CppMake_set(${name} "${value}" ON)
endmacro()

# Declare a string
macro(_string name value)
	_CppMake_check_var_available(${name})
	set(_CppMake_vars_${name}_type "bool")
	_CppMake_set(${name} "${value}" ON)
endmacro()

# Declare a list
macro(_list name type)
	_CppMake_check_var_available(${name})
	set(_CppMake_vars_${name}_type "list")
	
	# types: bool, int, string, mixed
	if(("${type}" STREQUAL "bool") OR ("${type}" STREQUAL "int") OR ("${type}" STREQUAL "string") OR ("${type}" STREQUAL "mixed"))
		set(_CppMake_vars_${name}_list_type "${type}")
	else()
		message(FATAL_ERROR "${type} is not a valid list type")
	endif()
endmacro()

#_list_push_front()
#_list_pop_back()
#_list_size()
#_list_sort()

# Declare an enumclass
macro(_enumclass name list)

endmacro()

macro(_enum name enumclass value)
endmacro()

# Loop through a list
macro(_foreach var name)
# Make sure var is the correct type
endmacro()

# casts a CppMake type to a plain CMake type
macro(_plaincast name)
endmacro()

# if using CppMake bool
macro(_if)
endmacro()

macro(test_cppmake)

	_bool(blah false)
	#_bool(blah2 crazy)
	_set(blah true)

	_int(blah2 5)
	#_int(blah3 h)
	#set(blah2 "fhele")
	_set(blah2 7)
	#_int(blah6 f3j21)

	_list(mylist string)
	#_list(myotherlist randomtype)

endmacro()