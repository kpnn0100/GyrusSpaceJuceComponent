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
    if (style == Slider::SliderStyle::RotaryHorizontalVerticalDrag)
    {
        drawRotarySlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, slider);
    }
}

void LookAndFeelOfTruePosition::drawLinearSliderThumb(Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&)
{
    //for removing upper class implement
}

void LookAndFeelOfTruePosition::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true);

    g.setColour(outline);
    g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            toAngle,
            true);

        g.setColour(fill);
        g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }

    auto thumbWidth = lineW;
    float thumbRadius = arcRadius - lineW;
    Point<float> centerPoint(x + width / 2, y + height / 2);
    Point<float> thumbPoint(bounds.getCentreX() + thumbRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY() + thumbRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    g.setColour(slider.findColour(Slider::thumbColourId));
    //g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
    g.drawLine({ centerPoint ,thumbPoint }, lineW/2);
}


Label* LookAndFeelOfTruePosition::createSliderTextBox(Slider& slider)
{
    Label* l = new Label;
    l->setFont(l->getFont().boldened());
    l->setJustificationType(Justification::centred);
    return l;
}


