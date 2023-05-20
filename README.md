# SimpleEQ
SimpleEQ is a lightweight equalizer VST plugin for audio processing. It provides a graphical user interface and real-time equalization capabilities for audio signals. It supports simple lowpass, highpass, and bandpass filters with controls for frequency, gain, and slope. This was based on a [video series](https://www.youtube.com/watch?v=i_Iq4_Kd7Rc) from MatKat Music demonstrating how to use the JUCE framework to create audio plugins.

<img width="598" alt="Screen Shot 2023-05-20 at 9 35 50 AM" src="https://github.com/colson0804/SimpleEQ/assets/5607871/ce13b81d-ea10-48d4-91e1-9c7b569f9d05">


## Getting Started 
### Prerequisites 
To use the SimpleEQ VST plugin, you need the following:
* A VST-compatible Digital Audio Workstation (DAW) or host software (such as JUCE's own AudioPluginHost) 
* C++ compiler with C++11 support
* JUCE framework (5.4 or higher)
* An IDE with a C++ build system (Xcode, Visual Studio, Cmake)

### Installation
Installation instructions are for MacOS. For Windows or Linux, you will need to follow different steps. 

To use the SimpleEQ plugin on MacOS, follow these steps: 
1. Clone the repository using Git. 
```shell
git clone https://github.com/colson0804/SimpleEQ.git
```
2. Open the SimpleEQ .jucer file 
3. From the jucer console, open the project in your preferred C++ IDE or text editor.
3. Build the project using the JUCE framework and your chosen build system (e.g., Xcode, Visual Studio, or CMake).
4. Once the build is successful, you will obtain the SimpleEQ VST plugin file (e.g., SimpleEQ.vst) in the output directory.

### Usage 
To use the SimpleEQ VST plugin, follow these general steps:

1. Open your VST-compatible DAW or VST host software.
2. Load the SimpleEQ VST plugin into your project or create a new track with the SimpleEQ plugin inserted.
3. Use the graphical user interface of the SimpleEQ plugin to adjust the equalizer parameters, such as gain and frequency.
4. Apply the equalization effect to the audio signal in real-time.

For more information on using VST plugins in your specific DAW or VST host software, please refer to the documentation or user guide provided by the software vendor.


