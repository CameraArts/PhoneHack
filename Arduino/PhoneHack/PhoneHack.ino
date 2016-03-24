/*
  Phone Hack installation for Laia Abril @Arles 2016
 by: Marco De Mutiis
 date: March 24, 2016
 
 required items:
 - Phone
 - MP3 Shield (+ Mp3 Shield Library & SDFat Library)
 - Arduino Uno
 
 phone hardware setup:
 ard 5V to red red wires
 ard GND to white wire next to red red wires
 ard A0 middle white wire
 
 Much of this code was grabbed from the FilePlayer and FileTrigger examples
 included with the SFEMP3Shield library. Major thanks to Bill
 Porter, Michael Flaga and Jim Lindblom.
 */

//////////////////////////////////////////////////////////////LIBRARIES
#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

//////////////////////////////////////////////////////////////OBJECTS & GLOBAL VARS
SdFat sd; // Create object to handle SD functions
SFEMP3Shield MP3player; // Create Mp3 library object
// These variables are used in the MP3 initialization to set up
// some stereo options:
const uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max
boolean lastStateDown = false; // This variable keeps track of the handset position


//////////////////////////////////////////////////////////////SETUP
void setup()
{

  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
  Serial.begin(115200); //initialize Serial for debugging
}


//////////////////////////////////////////////////////////////LOOP
void loop()
{
  //print out our value to check it's properly working 
  //(when the handset is picked up the value should be higher than 0)
  Serial.println(analogRead(A0)); 

  if ((analogRead(A0) != 0) && (lastStateDown == true)) //if the handset is picked up (and it was down before)
  {
    lastStateDown = false; //the handset is not down anymore

    // If the track was already playing, stop it
    if (MP3player.isPlaying()) MP3player.stopTrack();
    
    // Play the audio track (name has to be the same as the file name on the SD Card)
    MP3player.playMP3("TRACK001.mp3");

  }

  // if the handset is down
  if (analogRead(A0) == 0)
  {

    // If the track is still playing
    if (MP3player.isPlaying()) MP3player.stopTrack();
  }

  if(analogRead(A0) == 0) lastStateDown = true; //update the handset state

}

////////////////////////////////////////////////////////////initSD() 
//initializes the SD card and checks for an error.
void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
    sd.initErrorHalt();
  if(!sd.chdir("/")) 
    sd.errorHalt("sd.chdir");
}

/////////////////////////////////////////////////////////////initMP3Player() 
//sets up all of the initialization for the MP3 Player Shield. 
//It runs the begin() function, checks for errors, applies a 
//patch if found, and sets the volume/ stereo mode.
void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  if(result != 0) // check result, see readme for error codes.
  {
    // Error checking can go here!
  }
  MP3player.setVolume(volume, volume);
  MP3player.setMonoMode(monoMode);
}

