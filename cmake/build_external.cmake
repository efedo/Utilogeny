include(ExternalProject)
ExternalProject_Add(CopperSpiceExternal
	@PARSED_ARGS_PACKAGE_NAME@
	GIT_REPOSITORY    @PARSED_ARGS_GIT_REPOSITORY@ #https://github.com/copperspice/copperspice/ #git@github.com:FooCo/FooBar.git
	#GIT_TAG           origin/master
)