# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND vtgs_sources
    afskmod_impl.cc
    ax25_framer_impl.cc
    ax25_deframer_impl.cc
    ao40_fec_decoder.cc
    ao40_decoder_impl.cc
    afskdemod_impl.cc
    mult_scrambler_impl.cc
    mult_descrambler_impl.cc
    fsk96demod_impl.cc
    fsk96mod_impl.cc
    ao40_encoder_impl.cc)

set(vtgs_sources "${vtgs_sources}" PARENT_SCOPE)
if(NOT vtgs_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT vtgs_sources)

add_library(gnuradio-vtgs SHARED ${vtgs_sources})
target_link_libraries(gnuradio-vtgs ${Boost_LIBRARIES} ${PLANCK_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-vtgs PROPERTIES DEFINE_SYMBOL "gnuradio_vtgs_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-vtgs PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-vtgs
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_vtgs_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_vtgs.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_vtgs.cc
)

add_executable(test-vtgs ${test_vtgs_sources})

target_link_libraries(
  test-vtgs
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-vtgs
)

GR_ADD_TEST(test_vtgs test-vtgs)
