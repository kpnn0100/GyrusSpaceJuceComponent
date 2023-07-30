/*
  ==============================================================================

    ToggleButtonText.h
    Created: 30 Jul 2023 8:48:53pm
    Author:  PC

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;
class ToggleButtonText : public ToggleButton
{
public:
    ToggleButtonText();
private:
    Colour mHighlightColor = Colour(199, 112, 74);
    Colour mNormalColor = Colour(100, 100, 100).withAlpha(0.4f);
    void paint(Graphics& g) override;
};
