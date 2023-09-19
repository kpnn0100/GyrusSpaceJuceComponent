/*
  ==============================================================================

    LookAndFeelOfTruePosition.cpp
    Created: 19 Sep 2023 8:31:43pm
    Author:  PC

  ==============================================================================
*/

#include "LookAndFeelOfTruePosition.h"

void LookAndFeelOfTruePosition::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{
    if (style == Slider::SliderStyle::LinearHorizontal)
    {
        float strokeThickess = 2;

        float thumbWidth = height;
        double value = jmap<double>(slider.getValue(), slider.getRange().getStart(), slider.getRange().getEnd(), 0.0, 1.0);
        Point<float> centerOfThumb(value * (width - thumbWidth) + thumbWidth / 2, thumbWidth / 2);
        auto thumbBound = juce::Rectangle<float>(thumbWidth, thumbWidth);
        thumbBound.setCentre(centerOfThumb);

        auto fillRect = Rectangle<float>(width, height);
        auto sliderBound = fillRect.withSizeKeepingCentre(width - strokeThickess, height - strokeThickess);
        auto valueBound = sliderBound.withSizeKeepingCentre((sliderBound.getWidth() - thumbWidth) * value + thumbWidth, sliderBound.getHeight());
        valueBound.setX(strokeThickess / 2);

        g.setColour(slider.findColour(Slider::trackColourId));
        g.drawRoundedRectangle(sliderBound + Point<float>(x, y), sliderBound.getHeight() / 2, strokeThickess);

        g.setColour(slider.findColour(Slider::trackColourId));
        g.fillRoundedRectangle(valueBound + Point<float>(x, y), valueBound.getHeight() / 2);

        g.setColour(slider.findColour(Slider::thumbColourId));

        g.fillEllipse(thumbBound + Point<float>(x, y));
    }


}

void LookAndFeelOfTruePosition::drawLinearSliderThumb(Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&)
{
    //for removing upper class implement
}


Label* LookAndFeelOfTruePosition::createSliderTextBox(Slider& slider)
{
    Label* l = new Label;
    l->setFont(l->getFont().boldened());
    l->setJustificationType(Justification::centred);
    return l;
}


