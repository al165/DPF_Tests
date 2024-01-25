#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_NAME  "MyAmp"
#define DISTRHO_PLUGIN_URI   "https://github.com/REIS0/DPFTutorial"
#define DISTRHO_PLUGIN_CLAP_ID "studio.kx.distrho.examples.meters"

#define DISTRHO_PLUGIN_HAS_UI       1
#define DISTRHO_PLUGIN_IS_RT_SAFE   1
#define DISTRHO_PLUGIN_NUM_INPUTS   2
#define DISTRHO_PLUGIN_NUM_OUTPUTS  2
#define DISTRHO_PLUGIN_WANT_STATE   1
#define DISTRHO_UI_FILE_BROWSER     0
#define DISTRHO_UI_USER_RESIZABLE   0
#define DISTRHO_UI_USE_NANOVG       1

enum Parameters {
    kGainL = 0,
    kGainR,
    kOutL,
    kOutR,
    kParameterCount
};

#endif