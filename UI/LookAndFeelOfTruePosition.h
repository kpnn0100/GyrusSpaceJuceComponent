/*
  ==============================================================================

    LookAndFeelOfTruePosition.h
    Created: 19 Sep 2023 8:31:43pm
    Author:  PC

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;
class LookAndFeelOfTruePosition : public LookAndFeel_V4
{
public:
    void drawLinearSlider(Graphics&, int x, int y,
        int width, int height, float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle, Slider&) override;
    void drawLinearSliderThumb(Graphics&,
        int 	x,
        int 	y,
        int 	width,
        int 	height,
        float 	sliderPos,
        float 	minSliderPos,
        float 	maxSliderPos,
        const Slider::SliderStyle,
        Slider&
    ) override;
    void drawRotarySlider(Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, Slider&) override;
    Label* createSliderTextBox(Slider& slider) override;
};

