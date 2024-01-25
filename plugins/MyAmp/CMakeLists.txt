# CMake file for DISTRHO Plugins #
# ------------------------------ #

dpf_add_plugin(MyAmp
  TARGETS jack dssi lv2 vst2 vst3 clap
  FILES_DSP
      MyAmp.cpp
  FILES_UI
      MyAmpUI.cpp)

target_include_directories(
  MyAmp PUBLIC ".")
