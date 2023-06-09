//
//  ResponseCurveComponent.hpp
//  SimpleEQ
//
//  Created by Craig Olson on 4/7/23.
//

#import <JuceHeader.h>
#import "PluginProcessor.h"

enum FFTOrder {
    order2048 = 11,
    order4096 = 12,
    order8192 = 13
};

template<typename BlockType>
struct FFTDataGenerator
{
    void produceFFTDataForRendering(const juce::AudioBuffer<float>& audioData, const float negativeInfinity)
    {
        const auto fftSize = getFFTSize();
        
        fftData.assign(fftData.size(), 0);
        auto* readIndex = audioData.getReadPointer(0);
        std::copy(readIndex, readIndex + fftSize, fftData.begin());
        
        window->multiplyWithWindowingTable(fftData.data(), fftSize);
        
        forwardFFT->performFrequencyOnlyForwardTransform(fftData.data());
        
        int numBins = (int)fftSize / 2;
        
        for( int i = 0; i < numBins; ++i )
        {
            fftData[i] = juce::Decibels::gainToDecibels(fftData[i], negativeInfinity);
        }
        
        fftDataFifo.push(fftData);
    }
    
    void changeOrder(FFTOrder newOrder)
    {
        // when you change order, recreate the window, forwardFFT, fifo, fftData
        // also reset the fifoIndex
        // things that need recreating should be created on the heap via std::make_unique<>
        
        order = newOrder;
        auto fftSize = getFFTSize();
        
        forwardFFT = std::make_unique<juce::dsp::FFT>(order);
        window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris);
        
        fftData.clear();
        fftData.resize(fftSize * 2, 0);
        
        fftDataFifo.prepare(fftData.size());
    }
    
    //==============================================================================
    int getFFTSize() const { return 1 << order; }
    int getNumAvailableFFTDataBlocks() const { return fftDataFifo.getNumAvailableForReading(); }
    //==============================================================================
    bool getFFTData(BlockType& fftData) { return fftDataFifo.pull(fftData); }
private:
    FFTOrder order;
    BlockType fftData;
    std::unique_ptr<juce::dsp::FFT> forwardFFT;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window;
    Fifo<BlockType> fftDataFifo;
};

template<typename PathType>
struct AnalyzerPathGenerator
{
    void generatePath(const std::vector<float>& renderData,
                      juce::Rectangle<float> fftBounds,
                      int fftSize,
                      float binWidth,
                      float negativeInfinity)
    {
        auto top = fftBounds.getY();
        auto bottom = fftBounds.getHeight();
        auto width = fftBounds.getWidth();
        
        int numBins = (int)fftSize / 2;
        
        PathType p;
        p.preallocateSpace(3 * (int)fftBounds.getWidth());
        
        auto map = [bottom, top, negativeInfinity](float v)
        {
            return juce::jmap(v,
                              negativeInfinity, 0.f,
                              float(bottom), top);
        };
        
        auto y = map(renderData[0]);
        
        jassert(!std::isnan(y) && !std::isinf(y));
        
        p.startNewSubPath(0, y);
        
        const int pathResolution = 2;
        
        for( int binNum = 1; binNum < numBins; binNum += pathResolution )
        {
            y = map(renderData[binNum]);
            
            jassert(!std::isnan(y) && !std::isinf(y));
            
            if (!std::isnan(y) && !std::isinf(y))
            {
                auto binFreq = binNum * binWidth;
                auto normalizedBinX = juce::mapFromLog10(binFreq, 20.f, 20000.f);
                int binX = std::floor(normalizedBinX * width);
                p.lineTo(binX, y);
            }
        }
        
        pathFifo.push(p);
    }
    
    int getNumPathsAvailable() const
    {
        return pathFifo.getNumAvailableForReading();
    }
    
    bool getPath(PathType& path)
    {
        return pathFifo.pull(path);
    }
private:
    Fifo<PathType> pathFifo;
};

struct PathProducer
{
    PathProducer(SingleChannelSampleFifo<SimpleEQAudioProcessor::BlockType>& scsf) :
    fifo(&scsf)
    {
        fftDataGenerator.changeOrder(FFTOrder::order2048);
        monoBuffer.setSize(1, fftDataGenerator.getFFTSize());
    }
    
    void process(juce::Rectangle<float> fftBounds, double sampleRate);
    juce::Path getPath() { return fftPath; }
private:
    SingleChannelSampleFifo<SimpleEQAudioProcessor::BlockType>* fifo;
    
    juce::AudioBuffer<float> monoBuffer;
    
    FFTDataGenerator<std::vector<float>> fftDataGenerator;
    
    AnalyzerPathGenerator<juce::Path> pathProducer;
    
    juce::Path fftPath;
};

struct ResponseCurveComponent: juce::Component,
juce::AudioProcessorParameter::Listener,
juce::Timer
{
    ResponseCurveComponent(SimpleEQAudioProcessor&);
    ~ResponseCurveComponent();
    
    void parameterValueChanged (int parameterIndex, float newValue) override;

    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override { }
    
    void timerCallback() override;
    
    void paint (juce::Graphics& g) override;
    
    void resized() override;
    
    void toggleAnalysisEnabled(bool enabled)
    {
        shouldShowFFTAnalysis = enabled;
    }
    
private:
    SimpleEQAudioProcessor& audioProcessor;
    juce::Atomic<bool> parametersChanged { false };
    
    MonoChain monoChain;
    
    void updateChain();
    
    juce::Image background;
    
    juce::Rectangle<int> getRenderArea();
    
    juce::Rectangle<int> getAnalysisArea();
    
    PathProducer leftPathProducer, rightPathProducer;
    
    bool shouldShowFFTAnalysis = true;
};
