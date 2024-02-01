dpf_add_plugin(d_files
  TARGETS jack lv2 vst2
  FILES_DSP
      SamplePlayer.cpp
  FILES_UI
      SamplePlayerUI.cpp
      widgets/HBox.cpp
      widgets/Knob.cpp
      widgets/SimpleButton.cpp)

project(SamplePlayer)

target_include_directories(
  d_files PUBLIC "." "widgets")

# Set additional library directories
link_directories(/usr/lib)

# Add the SNDFile library
find_library(SNDFILE_LIBRARY NAMES sndfile)
if(SNDFILE_LIBRARY)
    message(STATUS "Sndfile library found: ${SNDFILE_LIBRARY}")
else()
    message(FATAL_ERROR "Sndfile library not found")
endif()

# Set the base flags
set(BASE_FLAGS -I. -I./widgets)

# Link the SNDFile library
target_link_libraries(SamplePlayer ${SNDFILE_LIBRARY})

# Set the link flags
set_target_properties(SamplePlayer PROPERTIES LINK_FLAGS "--libs=sndfile")
