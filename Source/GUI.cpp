/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Perc0Sampler.h"
#include "CSVLoader.h"
#include "FileUtilities.h"
#include "SonificationScheduler.h"
//[/Headers]

#include "GUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUI::GUI ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    PlayButton.reset (new TextButton ("Play Button"));
    addAndMakeVisible (PlayButton.get());
    PlayButton->addListener (this);

    comboBox.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox.get());
    comboBox->setEditableText (true);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addListener (this);

    vid_component.reset (new VideoComponent (false));
    addAndMakeVisible (vid_component.get());
    vid_component->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sonificationScheduler.setVideoSystem(&vid_component);
    StringArray all_names = sonificationScheduler.getAllNames();
    for(int i = 0; i < 10; i++){
        comboBox->addItem(TRANS(all_names.getReference(i)), i+1);
    }
    //[/Constructor]
}

GUI::~GUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    PlayButton = nullptr;
    comboBox = nullptr;
    vid_component = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        int x = proportionOfWidth (0.5000f) - (200 / 2), y = 12, width = 200, height = 30;
        String text (TRANS("SONIFICATION APP"));
        Colour fillColour = Colours::aqua;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    PlayButton->setBounds (proportionOfWidth (0.5016f) - (120 / 2), 112, 120, 56);
    comboBox->setBounds (proportionOfWidth (0.5016f) - (176 / 2), 48, 176, 48);
    vid_component->setBounds (proportionOfWidth (0.5016f) - (proportionOfWidth (0.9003f) / 2), 208, proportionOfWidth (0.9003f), proportionOfHeight (0.5000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == PlayButton.get())
    {
        //[UserButtonCode_PlayButton] -- add your button handler code here..
        //AudioDeviceManager audioDeviceManager;
        //perc0Sampler.playSample(Perc0Sampler::DRUM4);
        sonificationScheduler.startSonification();
        //vid_component->play();
        //CSVLoader loader = CSVLoader();
        //StringArray names = loader.getNames();
        //loader.getTs(names.getReference(6));
        //[/UserButtonCode_PlayButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void GUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        sonificationScheduler.setId(comboBox->getText());
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUI" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <TEXT pos="50%c 12 200 30" fill="solid: ff00ffff" hasStroke="0" text="SONIFICATION APP"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <TEXTBUTTON name="Play Button" id="16df8ca7730936ee" memberName="PlayButton"
              virtualName="" explicitFocusOrder="0" pos="50.161%c 112 120 56"
              buttonText="Play Button" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="4b23f54be60abb10" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="50.161%c 48 176 48"
            editable="1" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="new component" id="5ce758f777ff2e85" memberName="vid_component"
                    virtualName="" explicitFocusOrder="0" pos="50.161%c 208 90.032% 50%"
                    class="VideoComponent" params="false"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

