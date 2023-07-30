/*
  ==============================================================================

    ToggleButtonText.cpp
    Created: 30 Jul 2023 8:48:53pm
    Author:  PC

  ==============================================================================
*/

#include "ToggleButtonText.h"

ToggleButtonText::ToggleButtonText()
{
    repaint();
}

void ToggleButtonText::paint(Graphics& g)
{
    if (getToggleState())
    {
        g.setColour(mHighlightColor);
    }
    else
    {
        g.setColour(mNormalColor);
    }
    g.setFont(g.getCurrentFont().boldened());
    g.drawText(getTitle(), getLocalBounds(), Justification::centred, 1);
}
