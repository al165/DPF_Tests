#include "AITestUI.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

AITestUI::AITestUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<AITest *>(getPluginInstancePointer());

    Window &window = getWindow();

    _logo_font = createFontFromMemory("VG5000", VG5000_ttf, VG5000_ttf_len, false);

    hbox_controls = new HBox(this);

    // fGenerate = new Button(hbox_controls);
    // fGenerate->setLabel("generate");
    // fGenerate->setSize(100, 30);
    // fGenerate->setCallback(this);

    fBeatGrid = new BeatGrid(hbox_controls);
    fBeatGrid->setSize(Size<uint>(570, 160));
    fBeatGrid->setAbsolutePos(10, 110);
    fBeatGrid->_font = _logo_font;

    fThreshold = new VSlider(hbox_controls);
    fThreshold->setId(kThreshold);
    fThreshold->setSize(Size<uint>(20, 160));
    fThreshold->setCallback(this);
    fThreshold->gauge_width = 12.0f;
    fThreshold->max = 1.0f;
    fThreshold->foreground_color = Color(51, 51, 51);
    fThreshold->background_color = Color(255, 255, 255);
    fThreshold->marker_color = Color(0, 0, 0);

    fXYSlider = new XYSlider(hbox_controls);
    fXYSlider->setSize(Size<uint>(160, 160));
    fXYSlider->setCallback(this);
    fXYSlider->minX = -8.0f;
    fXYSlider->maxX = 5.0f;
    fXYSlider->minY = -2.5f;
    fXYSlider->maxY = 5.0f;
    fXYSlider->bg_color = Color(255, 255, 255);
    fXYSlider->fg_color = Color(51, 51, 51);
    fXYSlider->marker_color = Color(0, 0, 0);

    hbox_controls->setAbsolutePos(10, 50);
    hbox_controls->setWidth(UI_W);
    hbox_controls->padding = 10;
    hbox_controls->justify_content = HBox::Justify_Content::left;
    // hbox_controls->addWidget(fGenerate);
    hbox_controls->addWidget(fXYSlider);
    hbox_controls->addWidget(fBeatGrid);
    hbox_controls->addWidget(fThreshold);
    hbox_controls->positionWidgets();


    loadSharedResources();

    fBeatGrid->pattern = &plugin->pattern;

    setGeometryConstraints(UI_W, UI_H, true, true);
}

AITestUI::~AITestUI(){}

void AITestUI::parameterChanged(uint32_t index, float value)
{
    switch(index)
    {   
        case kThreshold:
            fThreshold->setValue(value);
            repaint();
            break;
        case kSixteenth:
            fBeatGrid->sixteenth = (uint8_t)value;
            fBeatGrid->repaint();
            break;
        case kEmbedX:
            fXYSlider->setXValue(value);
            fXYSlider->repaint();
            break;
        case kEmbedY:
            fXYSlider->setYValue(value);
            fXYSlider->repaint();
            break;  
        default:
            repaint();
            break;
    }
}

void AITestUI::stateChanged(const char *key, const char *value)
{
    printf("AITestUI::stateChanged()\n");

    repaint();
}

void AITestUI::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();

    beginPath();
    fillColor(Color(240, 240, 240));
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    char build_date[80];
    sprintf(build_date, "Build: %s %s", __DATE__, __TIME__);

    beginPath();
    fillColor(Color(40, 40, 40));
    fontSize(12);
    textAlign(Align::ALIGN_RIGHT);
    text(width-10, height-10, build_date, nullptr);
    closePath();

    beginPath();
    fillColor(Color(40, 40, 40));
    fontSize(32);
    textAlign(Align::ALIGN_RIGHT | Align::ALIGN_TOP);
    fontFaceId(_logo_font);
    text(width-10, 10, "waive", nullptr);
    closePath();
}



void AITestUI::buttonClicked(Button *button)
{
    if(button == fGenerate)
    {
        plugin->generateNew();
        repaint();
    }
}

void AITestUI::vSliderDragStarted(VSlider *vSlider){}

void AITestUI::vSliderDragFinished(VSlider *vSlider, float value){}

void AITestUI::vSliderValueChanged(VSlider *vSlider, float value)
{
    if(vSlider == fThreshold){
        setParameterValue(kThreshold, value);
    }
}

void AITestUI::xyDragStarted(XYSlider *xySlider){}

void AITestUI::xyDragFinished(XYSlider *xySlider, float x, float y){}

void AITestUI::xyValueChanged(XYSlider *xySlider, float x, float y)
{
    setParameterValue(kEmbedX, x);
    setParameterValue(kEmbedY, y);
    repaint();
}

UI *createUI()
{
    return new AITestUI();
}


END_NAMESPACE_DISTRHO