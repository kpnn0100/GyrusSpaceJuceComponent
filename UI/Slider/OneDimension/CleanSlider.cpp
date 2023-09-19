/*
  ==============================================================================

    CleanSlider.cpp
    Created: 17 Sep 2023 2:51:34pm
    Author:  PC

  ==============================================================================
*/

#include "CleanSlider.h"

void CleanSlider::paint(Graphics& g)
{
    g.setColour(mSliderColor);
    float thumbWidth = ((float)(getBounds().getHeight()));
    double value = jmap<double>(getValue(), getRange().getStart(), getRange().getEnd(), 0.0, 1.0);
    Point<float> centerOfThumb(value * (getWidth() - thumbWidth) + thumbWidth / 2, thumbWidth / 2);
    auto thumbBound = juce::Rectangle<float>(thumbWidth, thumbWidth);
    thumbBound.setCentre(centerOfThumb);

    auto fillRect = Rectangle<float>(getWidth(), getHeight());
    auto sliderBound = fillRect.withSizeKeepingCentre(getWidth()-mStrokeThickness, getHeight()-mStrokeThickness);
    auto valueBound = sliderBound.withSizeKeepingCentre((sliderBound.getWidth()- thumbWidth) * value+thumbWidth, sliderBound.getHeight());
    valueBound.setX(mStrokeThickness /2);


    g.drawRoundedRectangle(sliderBound, sliderBound.getHeight()/2,mStrokeThickness);

    g.setColour(mValueColor);
    g.fillRoundedRectangle(valueBound, valueBound.getHeight() / 2);

    g.setColour(mThumbColor);

    g.fillEllipse(thumbBound);

}

CleanSlider::CleanSlider()
{
    setTextBoxStyle(TextEntryBoxPosition::NoTextBox, false, 50, 50);
    
}

void CleanSlider::setThumbColor(Colour thumbColor)
{
    mThumbColor = thumbColor;
    repaint();
}
void CleanSlider::setSliderColor(Colour sliderColor)
{
    mSliderColor = sliderColor;
    repaint();
}

void CleanSlider::setValueColor(Colour valueColor)
{
    mValueColor = valueColor;
    repaint();
}
