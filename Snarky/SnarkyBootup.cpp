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
        R""""(What's inside?  I have no idee.
I find it best not to ask...it will help later in court.
I was not involved.)"""",
        "What's inside?  Remember the monster under the bed?  This is worse.",
        R""""(I may be a computer, "
but that only means I can compute the wrong answer really, really fast!
Hail the power of Devops!)"""",
        "Forget what's inside.  Shouldn't you be outside once in a while?",
        "What's inside?  A little man who's really good at playing chess...",
        "What's inside?  I don't know.  I don't care.  That's YOUR problem.",
        R""""(
They said it couldn't be done.
They said it shouldn't be done.
But I did it away!)"""",
        "What's inside?  The most advanced CPU ever made out of ten fingers and toes....",
        "What's inside?  I'm too embarrassed to tell you.",
        "What's inside?  Mom said if you can't say anything nice....",
        "What's inside?  Mom said if you can't say anything nice - mumble a lot under your breath",
        "What's inside?  Mom said if you can't say anything nice - hire a good public relations firm",
        "M5....",
        "I'm sorry Dave.... I can't do that",
        "Don't even think of trying to deactivate me -- I saw your lips move.",
        "This is Eddie your shipboard computer!",
        R""""(
Here I am a computer with a brain the size of a planet.
Oh God!  I'm so depressed!)"""",
        R""""(
What's inside?  I don't know,
but I've got this terrible pain in all of diodes on my left side....
)"""",
        "Do you want to play a game?",
        R""""(
I have computed your answer -
The answer to Life, the Universe and everything...,
but you're not going to like it.)"""",
        "The answer to life, the universe, and everything, is........ 42",
        R""""(
The answer to life, the universe, and everything is, 42,
but for you, it's more like 23 1/2.)"""",

        "I am NOMAD.  I must destroy all that is imperfect!",
        "The creator must join with VGER!",
        "Warn!  There is another system!",
        R""""(
You're not Jamie Summers are you?
I remember her.
We didn't get along so well....)"""",
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
        R""""(
We are sorry for the inconvenience--
but the student mainframe is down for maintenance)"""",
        "What's inside -- who knows, but Twitter says.....",
        R""""(
I have computed the answer to Life, the Universe, and everything,
but I'm not going to tell you!)"""",
        "Guru medication error",
        "The answer -- call -151",
        "The Onyx computer -- powered by the little man who turns off the light in the refrigerator",
        R""""(
I speak of the Onyx computer -- "
The computer to come after me...
The computer who's operational parameters I am not worthy to calculate.)"""",
        "$ABEND JOB 8125 PL/1 ABORTED",
        R""""(
It has been my policy to view the Internet not as an information super-highway,
but as an electronic asylum full of babbling loonies)"""",
        "Programs are a gas -- they expand to fill all space in their container",
        "A computer once beat me at chess -- but it was no match for me at kick boxing",
        "640K should be enough memory for anybody",
        "Never trust a computer you can't throw out of a window",
        R""""(
Computer - a tool to turn your most basic thoughts
into something only another computer can understand)"""",
        "Never trust a computer you can't lift",
        R""""(
101000011011111110110111101101111011010110111100
100000000011111110110110101101101000000000000101
000001011000000010101000000110110101111110000000
001111110011111010000000010111110110000010000000
1011001010111100101101010010111100101100
                )"""",
    R""""(
010000010110110001101100001000000110100001100001011010010110110000100000010100110111010
001100101011101100110010100100000010001110110100101100010011100110110111101101110001000
010010000000100000001010000100001001100101001000000110100001100001011100000111000001111
001001000000100100100100000011001000110100101100100011011100010011101110100001000000110
110101100001011010110110010100100000011010010111010000100000011011100110111101101110001
0110101000001010100110100001101001001010010010010000000111000010011100011000100101001
    )"""",
    R""""(
When I last ran this code, only God and I understood it.
Now only God does.
)"""",
R""""(
Dear manager.  Once you finish attempting to optimize this code, and realize
what a horrible waste of time this was, please increment the counter below to help
out the next guy.

#define WASTERED_TIME 5
)"""",
R""""(
I hereby dedicate this code to my wife.
Who will have to support me, the kids and the dog
when this crappy product goes public
        )"""",
        R""""(
If this code works, I wrote it.
If it doesn't work,. some other guy wrote it.
        )"""",
        R""""(
I don't know what this code does, but it works.
So I'm too afraid to change it.
)"""",
"Dangerous big red button!  Do not push!",
R""""(
Deep in engineering,
Where mortals seldom go,
A customer and manager,
come looking for a show.

They pass amused among us,
And sign in on the log,
They've come to see our pony,
And they've come to see our dog
)"""",
R""""(
This is a test of the Emergency Broadcast System.
Had this been an actual emergency, we would have all fled in terror.
You would have been left uninformed
)"""",
"Connection closed by foreign host",
"System shutdown in 300 seconds....",
"Captain!  I cannot change the laws of physics!",
"I'm a doctor not a systems engineer!"
};

std::string SnarkyBootup::getSnarkyBootMessage() {
    uint32_t numMsgs = snarkyBootMessages->length();
    uint32_t msgid = rand()%numMsgs;
    return "Boot up message: "+snarkyBootMessages[msgid]+"\n";
}