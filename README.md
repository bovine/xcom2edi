XCOM2EDI
========

![How much money do you want?](/images/money.gif)

What is X-COM?
--------------

"X-COM: Terror from the Deep" was a turn-based strategy game originally released by MicroProse
for MS-DOS computers in 1995. It continues to be relatively popular more than 20+ years later
among retro-gamers because of its complexity and difficulty.

For more information about the game, you can [check out its page on Wikipedia](https://en.wikipedia.org/wiki/X-COM:_Terror_from_the_Deep), 
or [its review page on Moby Games](https://www.mobygames.com/game/x-com-terror-from-the-deep).

The Windows version of the game is [available for play from Steam](http://store.steampowered.com/app/7650/XCOM_Terror_From_the_Deep/).


What is XCOM2EDI?
-----------------

Have you ever wanted to use a weapon that could blow a hole through any wall (or alien) with one blast?  Or how about kill
that last alien on the board that is always so difficult to locate?  Maybe you simply want to increase the armour level on
your soldiers or perhaps change their names to things easier to relate to?  What if you really liked your expert soldiers from
XCOM I and would simply like to pull them over into XCOM II to continue playing?

XCOM2EDI is a MS-DOS based application that will let you edit the saved game files created by XCOM-II and alter
various aspects of your game play.


Where can I download XCOM2EDI from?
-----------------------------------

Version 1.5 of the XCOM2EDI is available to the public for [download now](https://github.com/bovine/xcom2edi/releases/tag/v1.5)


What can it do?
---------------

Here is brief list of what this powerful editor can currently do:
* [money editor](images/money.gif) (enter whatever amount you like)
* inventory editor (edit the holdings of all of your bases)
* [base editor](images/buildings.gif) (lets you easily create buildings or complete their construction in no time at all!)
* [soldier editor](images/soldier.gif) (modify each of the stats of your soldiers, including their rank, name, stamina, 
   health, strength, etc.  including easy and immediate transporting of soldiers between bases.  You can also bring your
   dead soldiers back to life!)
* import your characters from XCOM I and bring them directly into XCOM II!
* ammo and weapon editors (edit the strengths, explosion types, cartridge size, etc. of the weapons and ammo used in the game)
* built-in backups/restores of your saved games for easy recovery in case you do something that you decide you don't want to doff.
* quickly gain full access to the ufopedia and all knowledge of alien technology in the game!
* tactical editor (lets you edit the health/stamina/strength/armour stengths/tu's/etc for all soldiers/enemies of save games
   made within tactical combats! Lets you easily give yourself an advantage (or disadvantage, if you want) in any battle!
* A complete revision of the weapon/ammo editors to yield a much more powerful object editor.
* An [enhanced tactical editor](images/tactical.gif) that affects game turns other than the current turn.
* Both the tactical editor and the object editor allow you to experiment with many more secret fields.
* A brand new alien containment editor to allow you to
      add/delete aliens to your containment facilities.
* A reveal map function that allows you to instantly be able to see everything around you.
* A [foreign relations editor](images/diplomacy.gif) which will allow you to get better financial support from other countries.
* A structures editor that will permit you to change the amount of money required to build or maintain buildings, 
   plus modify their vulnerability to attacks!
* A new submarine editor that allows you to change the weapons, ammo, fuel, and stocks that you have aboard all of your ships!
* fully mouse driven, making your editing very easy and intuitive. (Be sure to run the editor in full-screen mode if attempting to run it under Windows and you are having mouse control issues.)


What about the newer sequels/re-releases of X-COM?
--------------------------------------------------
This saved game editor does not support any other versions of X-COM, nor are there any plans to do so.


I'm having problems with my edited games
-----------------------------------------

Before you make changes to your saved games, you should make
sure that you use the backup functionality provided within the
editor.  This will invoke PKZIP to make a compressed backup of
your saved game.  This allows you to easily revert back to the
previous state if you notice that your saved game is no longer
working correctly.

You should also be aware that X-COM was not really designed to
operate with values as high as you are able to obtain through
the editor.  Because if this, you can may times cause X-COM's
values to overflow or loop around to zero (or a negative
number), if they increase too much.  You may also notice that
you can no longer run or throw grenades, or similar features if
you increase your player stats too much.

To avoid these problems, try to use a little bit of moderation
when you edit your saved game values.  Trying not to "max out"
all values in your player stats will usually prevent most of
these problems.  More details on these types of problems are
included in the documentation that comes with the editor.



Are registrations for XCOM2EDI still accepted?
----------------------------------------------

Registrations are no longer being accepted for XCOM2EDI as of April 1999.
However, since I have heard many people indicate how they still
believe that my editor is a viable program, I am providing the
registration information to fully unlock all of its features
below.  Please note that you must type in the information
exactly as shown below!  YOU CANNOT ENTER YOUR OWN NAME AND
PHONE NUMBER!

    Name: Registered User
    Phone: 1234567890
    Serial: 0M40J83E4JRR1ANHEAY2


What about the source code?
---------------------------

Many people have written to me asking for the source code to
my program.  Although I did not previously offer
source code during the time I was accepting shareware
registration fees, I am now willing to share my source since I
no longer care to receive the registration fees for either of
these old programs.  Programmers who understand C++ may be
interested in downloading the source code for either of these
editors.

Keep in mind that both of these programs are DOS-based programs,
so they have limited usefulness in the Windows-centric world of
today.  However, the text-based user interface and smooth-mouse
emulating code may be useful to others who are interested in
such legacy developments.  If you are interested strictly in the
text user-interface portion of my code you can also look in the 
`tui` subdirectory, which contains all of the headers and
source code associated with all of my mouse, buttons,
checkboxes, and message box handling code.

All of these programs were written using Borland C++ 4.x,
though I'm sure you could still get them to compile with Borland
C++ 5.5 that is now free for download.


