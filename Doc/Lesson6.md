# Lesson 6: GUI

In this lesson, we will learn how to display images and create different interface elements, such as buttons, for example. 
If you have not completed the first lessons, then I highly recommend that you do this.

## Step 1: Required Resources

To complete this lesson, we need a set of images and fonts that you can download from this link (zip archive, downloading from the official site of the engine).

## Step 2: Extract Resources

All resources from the archive must be extracted to the resources folder located in the folder with the engine.

## Step 3: Lua Scripting

So let's open our favorite init.lua. For this lesson we don’t need a scene, so we won’t download it.

```
gui = elf.CreateGui ()
elf.SetGui (gui)

while elf.Run () == true do
end
```

Now we have created a GUI interface and placed it in the gui variable with the elf.CreateGui () function and made it active using elf.SetGui (gui), the parameter of which is our just created interface.

## Step 4: Create a Picture

To get started, upload a static picture. Let it be the logo of the engine:

```
gui = elf.CreateGui ()
elf.SetGui (gui)

- create the elf logo
elflogotex = elf.CreateTextureFromFile ("resources / elf.png")

pic = elf.CreatePicture ("ElfLogo")
elf.SetPictureTexture (pic, elflogotex)
elf.SetGuiObjectPosition (pic, elf.GetWindowWidth () - elf.GetGuiObjectSize (pic) .x, 0)
elf.AddGuiObject (gui, pic)

while elf.Run () == true do
end
```

Do not be surprised at the line - create the elf logo, in Lua comments are separated from the code by two minuses "-".
What does this code do? The elf.CreateTextureFromFile function ("resources / elf.png") loads the image at the specified path into the elflogotex variable, and load it as a texture. 
Now create the picture itself with the elf.CreatePicture function. 
The parameter of this function is an arbitrary (you choose) name. 
Functions starting with Set / GetGuiObject * can be called for any GUI element, be it a button, picture, label, etc.

Now you can set the position of the picture on the screen. We will place it in the upper right corner. 
This will help us elf.SetGuiObjectPosition, the first parameter of which is our GUI element, the second is the x coordinate, where it should appear. 
At its location, we will stop and talk about this in more detail. 
Here we get the screen size through elf.GetWindowWidth and subtract the size of the image from it in x-coordinate so that it does not draw outside the screen. 
The third parameter is the y-coordinate. 
Here we just set 0.

Finally, we just need to add the GUI element to the GUI itself with the elf.AddGuiObject function. 
Its first parameter is a GUI interface into which we add an element, which is the second parameter. 
Important: do not forget to do this, otherwise the element simply does not appear on the screen.

If everything is correct, then after launch you will see a black screen with the engine logo in the upper right corner.

## Step 5: Create a Label

Label - An element that displays text. Let's create one, let it display FPS (Frames per second, frames per second):

```
gui = elf.CreateGui ()
elf.SetGui (gui)

- create the elf logo
elflogotex = elf.CreateTextureFromFile ("resources / elf.png")

pic = elf.CreatePicture ("ElfLogo")
elf.SetPictureTexture (pic, elflogotex)
elf.SetGuiObjectPosition (pic, elf.GetWindowWidth () - elf.GetGuiObjectSize (pic) .x, 0)
elf.AddGuiObject (gui, pic)

- add a label
font = elf.CreateFontFromFile ("resources / freesans.ttf", 12)

lab = elf.CreateLabel ("FPSLabel")
elf.SetLabelFont (lab, font)
elf.SetLabelText (lab, "FPS: 0")
elf.SetGuiObjectPosition (lab, 10, 10)

elf.AddGuiObject (gui, lab)

while elf.Run () == true do
elf.SetLabelText (lab, "FPS:" .. elf.GetFps ())
end
```

Here everything is similar, but now we still take the font. It is loaded with the elf.CreateFontFromFile function, where the font itself and its size will be the parameters. 
I think that the effect of the elf.SetLabelFont and elf.SetLabelText functions is obvious. 
The elf.SetLabelText (lab, "FPS:" .. elf.GetFps ()) function also appeared in the main loop, updating the displayed number of frames per second.

## Step 6: Other UI Elements

Now you know the basics of working with GUI elements, but we have not discussed all of them. Let's look at a few more:

Buttons:

```
- add exit button
exbtexoff = elf.CreateTextureFromFile ("resources / dsbexitoff.png")
exbtexover = elf.CreateTextureFromFile ("resources / dsbexitover.png")
exbtexon = elf.CreateTextureFromFile ("resources / dsbexiton.png")

exb = elf.CreateButton ("ExitButton")
elf.SetButtonOffTexture (exb, exbtexoff)
elf.SetButtonOverTexture (exb, exbtexover)
elf.SetButtonOnTexture (exb, exbtexon)
elf.SetGuiObjectPosition (exb, 10, 30)

exscr = elf.CreateScript ()
elf.SetScriptText (exscr, "elf.Quit ()")
elf.SetGuiObjectScript (exb, exscr)

elf.AddGuiObject (gui, exb)
```

Buttons have 3 textures: the usual state when the mouse is over the button and when the button is pressed.
The elf.SetScriptText function executes a script, which is the second parameter. 
In this case, the button will turn off the program. 
You can also use elf.CreateScriptFromFile to execute a script from a file.

Further, according to the plan, checkboxes, or in common people they are also called "checkmarks":

```
- create a check box
cbtexoff = elf.CreateTextureFromFile ("resources / dscboff.png")
cbtexon = elf.CreateTextureFromFile ("resources / dscbon.png")

cb = elf.CreateCheckBox ("MyCheckBox")
elf.SetCheckBoxOffTexture (cb, cbtexoff)
elf.SetCheckBoxOnTexture (cb, cbtexon)
elf.SetGuiObjectPosition (cb, 52, 30)

elf.AddGuiObject (gui, cb)
```

The checkbox has two textures: when it is turned on and when it is turned off

The last item on the list is a text input field:

```
- add text field
font = elf.CreateFontFromFile ("resources / freesans.ttf", 12)
txbxtex = elf.CreateTextureFromFile ("resources / dstxbx.png")

txf = elf.CreateTextField ("MyTextField")
elf.SetTextFieldFont (txf, font)
elf.SetTextFieldTexture (txf, txbxtex)
elf.SetTextFieldText (txf, "Some text ...")
elf.SetTextFieldOffset (txf, 4, 1)
elf.SetGuiObjectPosition (txf, 82, 30)

elf.AddGuiObject (gui, txf)
```

Here we downloaded the text and background, and then installed them in our field. 
Everything is obvious here, except elf.SetTextFieldOffset. 
This function sets the text offset relative to our element.

## Step 7: Events of the GUI Elements

Earlier, we registered the event for the button when it was pressed, but you can write a script for any interface element. 
Each element has its own cases of calls to your scripts. 
Buttons execute the script when pressed, text fields when typed, lost focus or activation. 
In order to find out what event happened, you can use the elf.GetGuiObjectEvent function. 
For example, you can catch activation of a text field like this:

```
if elf.GetGuiObjectEvent (me) == elf.GAIN_FOCUS then <some of your script>
```

Here is a list of events for different elements:
* Buttons - elf.CLICKED
* Text Fields - elf.GAIN_FOCUS, elf.LOSE_FOCUS, elf.CHAR_INPUT
* Sliders - elf.VALUE_CHANGED
* Text Lists - elf.SELECTION_CHANGED
* Check boxes - elf.STATE_CHANGED

Now you can make a menu of any complexity. 
I hope you enjoyed the tutorial :)