# Adonthell Main Stuff

set(adonthell_main_SRCS
	adonthell.cc
        main.cc
)

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})


# Create a shared library
add_library(adonthell_main SHARED ${adonthell_main_SRCS})
target_link_libraries(adonthell_main
	${PYTHON_LIBRARIES}
	)


################################
# Create the SDL Backend

## In function of the os, use a specific sdl wrapper.
if(APPLE)
	set(sdl_SRCS
		sdl/osx.m)
else(APPLE)
	set(sdl_SRCS
		sdl/linux.cc)
endif(APPLE)

add_library(sdl SHARED ${sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	adonthell_main
)