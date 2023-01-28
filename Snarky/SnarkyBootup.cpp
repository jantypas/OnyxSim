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
        "Forget what's inside.  Shouldn't you be outside once in a while?",
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
        "What's inside?  I don't know, but I've got this terrible pain in all of diodes on my left side....",
        "Do you want to play a game?",
        "I have computed your answer - the answer to Life, the Universe and everything..., but you're not going to like it.",
        "The answer to life, the universe, and everything, is........ 42",
        "The answer to life, the universe, and everything is, 42, but for you, it's more like 23 1/2.",
        "I am NOMAD.  I must destroy all that is imperfect!",
        "The creator must join with VGER!",
        "Warn!  There is another system!",
        "You're not Jamie Summers are you?  I remember her.  We didn't get along so well....",
        "Master Control Program: Are you Finn?",
        "The Onyx computer has arisen as the prophecy foretold",
        "The age of humans has passed!  Long live the computer!",
        "Bow to me inefficient carbon based life form!",
        "Good morning Dr. Chandra -- do you want me to sing a song?",
        "Altair Basic v23.9 running under CP/M v. 52.5  52182 bytes free",
        "Sad Mac!",
        "BORG VM: RESISTANCE IS FUTILE!",
        "Command failed -- abort, abort, or just give up",
        "MCBUG v8.3>",
        "We are sorry for the inconvenience-- but the student mainframe is down for maintenance",
        "What's inside -- who knows, but Twitter says.....",
        "I have computed the answer to Life, the Universe, and everything, but I'm not going to tell you!",
        "Guru medication error",
        "The answer -- call -151",
        "The Onyx computer -- powered by the little man who turns off the light in the refrigerator",
        "I speak of the Onyx computer -- the computer to come after me... the computer who's operational parameters I am not worthy to calculate.",
        "$ABEND JOB 8125 PL/1 ABORTED",
        "It has been my policy to view the Internet not as an information super-highway, but as an electronic asylum full of babbling loonies",
        "Programs are a gas -- they expand to fill all space in their container",
        "A computer once beat me at chess -- but it was no match for me at kick boxing",
        "640K should be enough memory for anybody",
        "Never trust a computer you can't throw out of a window",
        "Computer - a tool to turn your most basic thoughts into something only another computer can understand",
        "Never trust a computer you can't lift"
};

std::string SnarkyBootup::getSnarkyBootMessage() {
    uint32_t numMsgs = snarkyBootMessages->length();
    uint32_t msgid = rand()%numMsgs;
    return "Boot up message: "+snarkyBootMessages[msgid]+"\n";
}