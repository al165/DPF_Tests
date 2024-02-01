dpf_add_plugin(d_files
  TARGETS jack lv2 vst2
  FILES_DSP
      SamplePlayer.cpp
  FILES_UI
      SamplePlayerUI.cpp
      widgets/HBox.cpp
      widgets/Knob.cpp
      widgets/SimpleButton.cpp)

target_include_directories(
  d_files PUBLIC ".")