//
//  LookAndFeel.h
//  SimpleEQ
//
//  Created by Craig Olson on 4/7/23.
//

#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics&, int x, int y, int width,
                          int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;
    
    void drawToggleButton (juce::Graphics& g,
                           juce::ToggleButton& toggleButton,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;
};
