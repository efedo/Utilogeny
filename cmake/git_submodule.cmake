if(GIT_SUBMODULE_SETUP)
	return()
endif()

find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")

	# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Git submodules updating")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
	
	function(git_clone submodulename submodulelocation)
		if(submodulename AND submodulelocation)
			set(GIT_CLONE_PATH "${PROJECT_SOURCE_DIR}/lib/extern/" CACHE INTERNAL "")
			execute_process(COMMAND ${GIT_EXECUTABLE} clone "${submodulelocation}" "${GIT_CLONE_PATH}${submodulename}/"
							WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
							RESULT_VARIABLE GIT_SUBMOD_RESULT)
			if(NOT GIT_SUBMOD_RESULT EQUAL "0")
				message(FATAL_ERROR "git clone failed with ${GIT_SUBMOD_RESULT}, for package : ${submodulename} from: ${submodulelocation}")
			endif()
		else()
			message(FATAL_ERROR "Invalid Git clone request. Submodule : ${submodulename} from: ${submodulelocation}")
		endif()
	endfunction()
	
	set(GIT_SUBMODULE_SETUP "TRUE")
else()
    message(FATAL_ERROR "Git submodule update failed in project dir: ${PROJECT_SOURCE_DIR}.")
endif()

