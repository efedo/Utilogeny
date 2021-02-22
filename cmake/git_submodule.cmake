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
	
	function(git_submodule_download submodulename submodulelocation)
		if(submodule AND submodulename)
			execute_process(COMMAND ${GIT_EXECUTABLE} submodule add \"${submodulelocation}\" \"${PROJECT_SOURCE_DIR}/lib/extern/${submodulename}\"
							WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
							RESULT_VARIABLE GIT_SUBMOD_RESULT)
			if(NOT GIT_SUBMOD_RESULT EQUAL "0")
				message(FATAL_ERROR "git submodule add failed with ${GIT_SUBMOD_RESULT}, for submodule : ${submodulename} from: ${submodulelocation}")
			endif()
		else()
			message(FATAL_ERROR "Invalid Git submodule download request. Submodule : ${submodulename} from: ${submodulelocation}")
		endif()
	endfunction()
	
	set(GIT_SUBMODULE_SETUP)
else()
    message(FATAL_ERROR "Git submodule update failed in project dir: ${PROJECT_SOURCE_DIR}.")
endif()

