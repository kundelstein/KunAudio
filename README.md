# KunAudio
The sound system from KunEngine.


1. About

This is the audio system of my engine. It can be used as a standalone framework, though you should clean up some code then. It is used like this mostly to be coherent with the rest of the engine.

At first, I was looking for some system from the internet but I noticed very bad design decisions. 
- Here we use audio sources only for playback, when the sound finishes, we get back our source.
- Furthermore, The system can be paused, some other part of the game can launch and use audio for its own sounds and when we get back, we can continue playing our sounds.
- We use priorities of sounds, this helps us to choose which sound to interrupt if we run out of channels.
- Probably more but I forgot.


2. Design decisions

The audio system follows some rules, which are forced upon the user to keep some order. The sound system is designed to be used for sounds, not for music. The sound will stop (or freezes) if your game stops, as it should. Your game will probably use another sound system for music playback anyway to limit the use of channels. 


I don't use any threads here, so either use your own thread or don't use them at all. The result of not using threads is that looping sound with signal callback will have a very small pause because the engine will have to process the callback event.

Mixer objects - created as the container for Sound objects. You can add many Sound objects to the single Mixer. You can create many Mixers, which in real life scenario will be a case. The whole idea of Mixers is that you can use them for certain parts of your game which behave differently (ex. have a different volume or can be turned on and off depending on the situation).

SoundEffect objects - used for short sounds, which are playing sounds from the memory.

SoundStream objects - used for longer sounds, which are streamed from a hard drive (or other storage).

SoundEffectFile - is storing the sound data which will be used by SoundEffect.

You can attach callbacks to the sounds, though do not rely game logic on that - remember that if someone doesn't have a sound card, the sound won't be played and you won't get any callbacks. Maybe I should change that in future and use parallel timers but there was no need for that yet.


3. Dependences

STB VORBIS
OPENAL / OPENAL SOFT
GLM

I usually put them in ext folder.


4. Code quality

It works. Some parts are far from perfect but nothing that will crash your game if you are careful. The system is not really cache-friendly and should be tweaked to use DOD. The engine is still in development and some decisions are easier to make if things are made in a primitive way.


5. Portability

I run this system on Windows and XCode.


6. I'm not an octopus

I don't have 6 hands (the other 2 are apparently legs). If you spot or improve something, there are here in github ways to let me know.

