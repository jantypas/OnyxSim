//
// Created by jantypas on 1/17/23.
//

#include "SnarkyBootup.h"
#include <string>
#include <vector>

std::vector<std::string> snarkyBootMessages = {
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
        R""""(
What's inside?
I have no idee.
I find it best not to ask...
it will help later in court to say I was not involved.
)"""",
R""""(
What's inside?
Remember the monster under the bed?
This is worse.
)"""",
        "Don't be afraid -- be very afraid",
R""""(
I may be a computer,
but that only means I can compute the wrong answer really, really fast!
Hail the power of Devops!
)"""",
        "Forget what's inside.  Shouldn't you be outside once in a while?",
        "What's inside?  A little man who's really good at playing chess...",
        "What's inside?  I don't know.  I don't care.  That's YOUR problem.",
        R""""(
They said it couldn't be doneT
They said it shouldn't be done.
But I did it away!
So there!!

)"""",
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
Oh God!  I'm so depressed!
)"""",
R""""(
What's inside? I don't know,
but I've got this terrible pain in all of diodes on my left side....
)"""",
        "Do you want to play a game?",
        R""""(
I have computed your answer -
The answer to Life, the Universe and everything...,
but you're not going to like it.
)"""",
        "The answer to life, the universe, and everything, is........ 42",
        R""""(
The answer to life, the universe, and everything is, 42,
but for you, it's more like 23 1/2.
)"""",
        "I am NOMAD.  I must destroy all that is imperfect!",
        "The creator must join with VGER!",
        "Warn!  There is another system!",
        R""""(
You're not Jamie Summers are you?
I remember her.
We didn't get along so well.)"""",
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
        "$ABEND JOB 8125 PL/1 ABORTED",
        R""""(
  It has been my policy to view the Internet not as an information super-highway,
bubut as an electronic asylum full of babbling loonies",
)"""",
        "Programs are a gas -- they expand to fill all space in their container",
        "A computer once beat me at chess -- but it was no match for me at kick boxing",
        "640K should be enough memory for anybody",
        "Never trust a computer you can't throw out of a window",
        "Computer - a tool to turn your most basic thoughts into something only another computer can understand)",
        "Never trust a computer you can't lift",
        "When I last ran this code, only God and I understood it. Now only God does.",
        R""""(
Dear manager.  Once you finish attempting to optimize this code,
and realize what a horrible waste of time this was,
please increment the counter below to help out the next guy.

##define WASTERED_TIME 5)"""",
        R"""(
I hereby dedicate this code to my wife.
Who will have to support me,
the kids and the dog
when this crappy product goes public.)""",
        R""""(
If this code works, I wrote it.
If it doesn't work,. some other guy wrote it
I was just stuck trying to fix a flaming mess.)"""",
        R""""(
I don't know what this code does, but it works.
So I'm too afraid to change it.)"""",
    "Dangerous big red button!  Do not push!",
    R""""(
Deep in engineering,
Where mortals seldom go,
A customer and manager,
come looking for a show.

They pass amused amon us,
And sign in on the log,
They've come to see our pony,
And they've come to see our dog)"""",
    R""""(
This is a test of the Emergency Broadcast System.
Had this been an actual emergency, we would have all fled in terror.
You would have been left uninformed)"""",
    "Connection closed by foreign host",
    "System shutdown in 300 seconds....",
    "Captain!  I cannot change the laws of physics!",
    "I'm a doctor not a systems engineer!",
    "RFI = Request from Idiots",
    "RFP = Request Fantasy Product",
    "RFP = Request Fantasy Pricing",
    R""""(
As sung in Cats!)

Midnight!
Still waiting for the compiler....
Have to fix all the errors....
In this guy's crappy code!)"""",
R""""(
Welcome to the Real IT School Academy...
Now today, we're introducing you to one of our senior members -- Ralph.
Ralph is a real pro - he's eliminated more productivity in his years than most.
Did you see how he started system backups AND virus scanning right before the code deadlines?)"""",
    "This feature -- CPU..... does not work.  Please make a note of it",
    R""""(
// RJB: This code is bad, but it works, and it's only temporary until it's fixed
// AKD: Temporary like hell == the pyramids probably have a note that said
// Pharaoh, I know this is a hack, but we'll fix it in the next release....,
)"""",
R""""(
// AJK::// Kansas City Cassette Emulator
// OK -- it's ancient, and we're not even using real cassettes,
// but the code works
// It's easier to emulate the 70s computer than to figure out what that code does.
// The original programmer isn't available anymore -- I think he's dead.
// RJK: Dead?  Or does he just feel that way?  I feel like that all the time.
// AJK: No, I think he's really dead.
// RJK: Well I guess that precludes us brining him back on a contract?
// AJK: Don't know -- he could be working for the government now -- it's hard to tell.
)"""",
    "Diogenes was in search of an honest man -- I'm in search of an insane woman -- Rich Rozen",
    R""""(
Oh...
A host is a host,
From coast to coast,
And no one will talk
to a host that's close,
Unless that host,
that isn't close,
is busy, hang or dead
)"""",
    "If these messages are too tame for you -- try fortune -o",
    "Mr. Spock -- you've got the con",
    R""""(
I speak of the Onyx Computer.
The computer that is to come after me.
The computer whpse merest operational parameters,
I am not worthy to calculate.
)"""",
"If you see this message, abandon all hope -- even if you don't see this message, abandon all hope",
"You really think your code will work this time don't you?",
"What's inside?  It's your fault isn't it?  Admit it!",
"Don't you feel guilty dumping this code upon the world?",
"What's inside is not important.  Why it's there is the question.",
"This message intentionally left blank",
"What's inside?  My lawyer says I don't have to answer",
"Insufficient funds",
"Please hold, your message is important to us",
R""""(
Three things to be wary of
A new kid in his prime
A man with all the answers
And code that runs... first time
)"""",
R""""(
This code teaches assembly language.
Work at it and someday,
you could be as good as Steve Gibson

(You don't even know who Steve Gibson is do you?')
Go to http://www.grc.com and learn something!)"""",
"99 dead routines.... crashing on my machine...",
"This machine doesn't actually run code -- more often it stumbles around it",
R""""(
The Onyx1 computer.
The worlds first undefined instruction set.
Some are complex, sore are reduced.
Others are working on it with SlimFast but aren't there yet.
Others claim to be getting there but sneak cookie dough when you're not looking.
)""""
};

std::string SnarkyBootup::getSnarkyBootMessage() {
    uint32_t numMsgs = snarkyBootMessages.size();
    uint32_t msgid = rand()%numMsgs;
    std::string result = "\033[1;34mGuess what!\n"
                         "Out of all "+std::to_string(numMsgs)+" bits of wisdom we could have chosen....\n"+
            "The magic eight ball of wisdom chose this!!  Wisdom #"+std::to_string(msgid)+"\n\033[1;32m"+snarkyBootMessages[msgid]+
            "\33[1;0m";
    return result;
}