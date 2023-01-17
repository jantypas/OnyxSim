//
// Created by jantypas on 1/17/23.
//

#include "SnarkyBootup.h"
#include <string>

std::string snarkyBootMessages[] = {
        "What's inside?  You really don't want to know.",
        "Nearly 100 (somewhat) correct opcodes!",
        "It's inside -- don't let it out!  You have no idea what it takes to catch it.",
        "Now with more blue smoke power!",
        "The computer your mother wouldn't let you buy when you were a kid.",
        "What's inside? If you're smart you'll step on it before it gets to the children!",
        "It's inside -- what did you expect for free?  It might even work!",
        "Whatever is inside, it's not my fault. I had nothing to do with this!",
        "Yeah, it's inside -- but that's nothing to be proud of.",
        "It's inside, but doctors have medication for it now...",
        "What's inside?  You look.... I'm too scared.",
        "What's inside?  I have no idee.  I found it best not to ask...it will help later in court.  I was not involved.",
        "What's inside?  Remember the monster under the bed?  This is worse.",
        "I may be a computer, but that only means I can compute the wrong answer really, really fast = the power of Devops!",
        "Forget what's inside.  Should you be outside once in a while?",
        "What's inside?  A little man who's really good at playing chess...",
        "What's inside?  I don't know.  I don't care.  That's YOUR problem.",
        "They said it couldn't be done.  They said it shouldn't be done.  But I did it away!",
        "What's inside?  The most advanced CPU ever made out of ten fingers and toes....",
        "What's inside?  I'm too embarrassed to tell you.",
        "What's inside?  Mom said if you can't say anything nice....",
        "What's inside?  Mom said if you can't say anything nice - mumble a lot under your breath",
        "What's inside?  Mom said if you can't say anything nice - hire a good public relations firm",
        "M5....",
        "I'm sorry Dave.... I can't do that",
        "Don't even think of trying to deactivate me -- I saw your lips move.",
        "This is Eddie your shipboard computer!",
        "Here I am a computer with a brain the size of a planet.  Oh God!  I'm so depressed!",
        "What's inside?  I don't know, but I've got this terrible pain in all of diodes on my left side...."
};

std::string SnarkyBootup::getSnarkyBootMessage() {
    uint32_t numMsgs = snarkyBootMessages->length();
    uint32_t msgid = rand()%numMsgs;
    return snarkyBootMessages[msgid];
}