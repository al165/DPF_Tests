dpf_add_plugin(
  AITest
  TARGETS 
      # jack
      lv2 
      vst3
      vst2
      clap
  FILES_DSP
      AITest.cpp
  FILES_UI
      AITestUI.cpp
      widgets/HBox.cpp
      widgets/Knob.cpp
      widgets/SimpleButton.cpp
      widgets/BeatGrid.cpp
      widgets/XYSlider.cpp
      widgets/VSlider.cpp
      font.cpp
)

target_include_directories(
  AITest 
  PUBLIC 
    .
    widgets
    models
)