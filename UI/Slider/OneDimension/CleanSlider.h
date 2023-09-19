/*
  ==============================================================================

    CleanSlider.h
    Created: 17 Sep 2023 2:51:34pm
    Author:  PC

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <sstream>
#include <iomanip>
using namespace juce;
class CleanSlider : public juce::Slider
{
private:

    Colour mThumbColor = Colour::fromRGB(222,222,222);
    Colour mSliderColor = Colour::fromRGB(100, 100, 100);
    Colour mValueColor = Colour::fromRGB(175, 175, 175);
    double mStrokeThickness = 2;
    void paint(Graphics& g) override;
public:
    CleanSlider();
    void setThumbColor(Colour thumbColor);
    void setSliderColor(Colour sliderColor);
    void setValueColor(Colour valueColor);
};

