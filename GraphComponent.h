#pragma once

#include <JuceHeader.h>

//==============================================================================
class GraphComponent : public juce::Component
{
public:
    GraphComponent() : data(nullptr), dataSize(0) {}
    
    // Set the data to display
    void setData(const float* dataPtr, int size)
    {
        data = dataPtr;
        dataSize = size;
        repaint();
    }
    
    void paint(juce::Graphics& g) override
    {
        // Draw background
        g.fillAll(juce::Colours::black);
        
        if (data == nullptr || dataSize <= 0)
            return;
        
        // Find min and max values for scaling
        float maxVal = data[0];
        for (int i = 0; i < dataSize; ++i)
        {
            maxVal = juce::jmax(maxVal, data[i]);
        }
        float minVal = 0.0f;
        
        // Prevent division by zero
        if (maxVal == minVal)
            maxVal = minVal + 1.0f;
        
        float range = maxVal - minVal;
        float width = (float)getWidth();
        float height = (float)getHeight();
        
        // Y-axis label area width
        int yAxisLabelWidth = 30;
        int topPadding = 10;
        int bottomPadding = 10;
        
        float graphLeft = yAxisLabelWidth;
        float graphRight = width;
        float graphTop = topPadding;
        float graphBottom = height - bottomPadding;
        
        float graphWidth = graphRight - graphLeft;
        float graphHeight = graphBottom - graphTop;
        
        // Draw Y-axis
        g.setColour(juce::Colours::white);
        g.drawLine(graphLeft, graphTop, graphLeft, graphBottom, 1.0f);
        
        // Draw Y-axis scale labels (5 scale points)
        g.setColour(juce::Colours::lightgrey);
        g.setFont(juce::Font(10.0f));
        
        for (int i = 0; i <= 4; ++i)
        {
            float normalizedPos = i / 4.0f;
            float yPos = graphBottom - (normalizedPos * graphHeight);
            float value = minVal + (normalizedPos * range);
            
            // Draw tick marks
            g.setColour(juce::Colours::white);
            g.drawLine(graphLeft - 4, yPos, graphLeft, yPos, 1.0f);
            
            // Draw scale labels
            g.setColour(juce::Colours::lightgrey);
            juce::String labelText = juce::String(value, 2);
            g.drawText(labelText, 
                      0, (int)yPos - 7, yAxisLabelWidth - 4, 14,
                      juce::Justification::right, false);
        }
        
        // Draw vertical bars
        if (dataSize > 0)
        {
            float barWidth = graphWidth / dataSize;
            float barPadding = barWidth * 0.1f; // 10% padding on each side
            float actualBarWidth = barWidth - (2 * barPadding);
            
            g.setColour(juce::Colours::grey);
            
            for (int i = 0; i < dataSize; ++i)
            {
                float normalizedValue = (data[i] - minVal) / range;
                float barHeight = normalizedValue * graphHeight;
                
                float barX = graphLeft + (i * barWidth) + barPadding;
                float barY = graphBottom - barHeight;
                
                g.fillRect(barX, barY, actualBarWidth, barHeight);
            }
        }
    }
    
private:
    const float* data;
    int dataSize;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphComponent)
};
