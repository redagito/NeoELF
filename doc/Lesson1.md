# Lesson 1: The Basics

Translated from https://gcup.ru/publ/engines/blendelf_urok_1_osnovy/2-1-0-270

In this tutorial, we will learn the basics of working with the wonderful BlendELF engine.

## Step 1: Resources Needed for a Lesson

So, I hope you have already downloaded the BlendELF Development Kit. 
If not, no trouble! You can download it from http://blendelf.com. 
The BlendELF Development Kit contains the executable files of the engine, its libraries and other files that we will use in this lesson.

## Step 2: Extract the engine files

Extract the engine files to any empty folder. 
My engine folder looks like this:

![Lesson_1_1](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson1_1.jpg)

Your folder may look different, depending on the operating system you are using (Ubuntu 10.04 in the screenshot) and the version of the engine, but the essence of this does not change. 
Also, the display of the file extension (.exe, .dll, etc.) may be turned off on your system, but this does not create a problem either, does it? ;)

Here is a short list of files that must be in the folder:
* BlendELF.exe and DLLs - used to run the engine version for Windows
* blendelf files in the linux_libraries folder - used to launch the version of the engine for Linux
* LICENSE.txt - A file containing the license of the engine and libraries used in it
* The tools folder contains the scene exporter for Blender
* The docs folder contains a description of the lua API engine
* The cppsdk folder contains files for working with the C ++ engine
* The dotnetsdk folder contains files for working with the engine in .NET languages
* The levels folder contains examples of game scenes (levels) for the engine

## Step 3: Game Level

In the levels folder there should be a file called level1.pak. 
Files with the PAK extension in BlendELF are used as scene files (levels) that contain game resources used at this level: textures / models / objects / cameras / lighting and everything that can be in your game. 
Let's load this level into the game. 
Transfer the file level1.pak to the main directory of the engine (where the BlendELF.exe and blendelf files are located)
It will look something like this:

![Lesson_1_2](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson1_2.jpg)

## Step 4: init.lua script

Naturally, we can’t do without programming. 
You can program in BlendELF in the scripting language Lua, in C ++ and in .NET. 
First, we’ll learn how to use scripts in the Lua engine. 
Create a text file with any text editor in the folder with the engine and name it init.lua.
Note: make sure that it does not bear the name init.lua.txt, otherwise the engine simply will not see it:

![Lesson_1_2](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson1_3.jpg)

Stop stop stop! What is init.lua ?!

Well, that was asked. 
The init.lua file is a script that we will write later. 
It will be the heart of your future game, it will contain all the game logic, you will simply work in it (if you don’t take the dark path C ++, of course wink)

Lua ?! Never heard...

Lua is a very simple scripting language. 
It is very often used in the gaming industry. 
The combination of simple syntax, great functionality and good portability makes it a great choice for developers. 
You can also find more information about Lua on the official website: http://lua.org

## Step 5: The Easiest Lua Script

So, let's write something to our newly created file! 
open init.lua with any text editor, be it notepad, nano or any other. 
A game consisting only of an empty black room where nothing is invisible is not a very good option, is it? 
Therefore, the first thing we must do is load our level into the game. 
This can be done with the elf.LoadScene function. 
This function has one parameter - this is the path to your PAK file. 
So, here is the first line of our script:

```
elf.LoadScene ("level1.pak")
```

Now the script loads the scene. But when you start the game immediately closes, and we do not have time to see anything! 
No problem! You just need to “loop” it. 
To do this, create a while loop. 
In the future, most of your code will be executed in this loop. 
Add the lines of the loop after loading the level and get the following code:

```
elf.LoadScene ("level1.pak")
while elf.Run () == true do
end
```

Great, we wrote the code in init.lua, but before starting the game, we’ll first figure out what we did:
while is a loop that will run while the elf.
Run function returns TRUE. 
every time after the cycle has worked, it checks whether elf.Run will return TRUE. 
if so, then the cycle will run anew. the do statement starts the execution of the loop body. 
The end statement indicates that the cycle should begin again (namely, by checking elf.Run) when all of its statements are complete. 
elf.Run returns TRUE while the engine window is running and it works without fail. 
If you need to quit the game, call the elf.Quit function. it can be called anywhere in your game and it will force elf.Run to accept FALSE. 
this value is also automatically taken when the game window is closed.

## Step 6: Launch Your Games

Finally, we can now run the executable file (BlendELF.exe for windows or blendelf for linux)

But the game did not start! 
A window appeared and immediately disappeared!

There may be several options. 
The most banal and the most impossible is that your computer does not support OpenGL 2.0. you could also make a mistake by driving code into the init.lua script. 
Another option is that you do not have a level file in the folder with the executable file, so the engine finds it. 
To find out for sure what is the cause of the error, you can open the elf.log file. 
There certainly the reason is written. 
If the error is not clear to you - try to do the lesson first, maybe you will come across what led you to the error. 
If everything is correct and the game has started, then you will see something like this:

![Lesson_1_2](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson1_4.jpg)