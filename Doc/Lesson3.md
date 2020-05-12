# Lesson 3: Camera Control

In this tutorial we will do camera control

## Step 1: WSAD

The first thing we should do is change our main loop, which is in the init.lua file. It should look like this:

```
scn = elf.LoadScene ("level1.pak")
while elf.Run () == true do
end
```

Why do we need the scn variable before elf.LoadScene? We have not used it in previous lessons!

Yes, she was not required in previous lessons. The elf.LoadScene function will return the handle of the loaded level to this variable, and we will be able to access it by this variable.
Now we must make the active camera the same way:

```
scn = elf.LoadScene ("level1.pak")
cam = elf.GetSceneActiveCamera (scn)
while elf.Run () == true do
end
```

As you can see, we can pass the scn variable to the elf.GetSceneActiveCamera function to activate the camera in this particular scene.
Now we have a camera enclosed in a cam variable, so what next? and then we need to register a code for reading keystrokes from the keyboard. let's do this for the W button, which we will use to move the camera forward.

```
scn = elf.LoadScene ("level1.pak")
cam = elf.GetSceneActiveCamera (scn)
while elf.Run () == true do
if elf.GetKeyState (elf.KEY_W) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, -12.0)
end
end
```

elf.GetKeyState returns the state of the key. in our case, elf.KEY_W points to the key W. This function can return the following values:

* elf.UP - button not pressed
* elf.PRESSED - the button has been pressed and released
* elf.DOWN - button is pressed and not released
* elf.RELEASED - the button has been released

We now have the elf.MoveActorLocal function, which moves the camera. this function can also be applied to any game object. The word Local in the function name means that we move the object in its coordinate axes (local), and not in the axis of the scene (global). To offset an object in the global axes, the elf.MoveActor function is used. The numbers that are the arguments to this function are the coordinates of the displacement of the object per unit time.
Then why in the argument we indicated a change in the coordinate along the Z axis, and the camera moves forward?
Because the direction of the axes of the objects is different from the direction of the axes of the level. in theory, Z will move the object forward, Y up, and X to the right / left. I advise you to remember this, in order to avoid mistakes in the future, wink also do not forget that for elf.MoveActor the offset will already be on the global game axes.
Try now to test the game. If the camera does not move, look for an error in the log or console window.
Now let's add our program to control the buttons A, S, D. The code will be similar:

```
elf.LoadScene ("level1.pak")
cam = elf.GetSceneActiveCamera (scn)
while elf.Run () == true do
if elf.GetKeyState (elf.KEY_W) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, -12.0)
end

if elf.GetKeyState (elf.KEY_S) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, 12.0)
end

if elf.GetKeyState (elf.KEY_A) == elf.DOWN then
elf.MoveActorLocal (cam, -12.0, 0.0, 0.0)
end

if elf.GetKeyState (elf.KEY_D) == elf.DOWN then
elf.MoveActorLocal (cam, 12.0, 0.0, 0.0)
end
end
```

Again, you can test the game.

## Step 2: Mouse Control

Now let's teach the camera to respond to the mouse. The cursor will have to be in the game window all the time, but it should not be visible. We hide it with the elf.HideMouse function, whose argument is TRUE. But now we can’t close the game window by clicking the cross! So, you need to add the exit code for the game by pressing the Esc key.
Of course, you can close the game by pressing the F10 function key, but still ... so, here is our code:

```
elf.HideMouse (true)
scn = elf.LoadScene ("level1.pak")
cam = elf.GetSceneActiveCamera (scn)
while elf.Run () == true do

if elf.GetKeyState (elf.KEY_W) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, -12.0)
end

if elf.GetKeyState (elf.KEY_S) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, 12.0)
end

if elf.GetKeyState (elf.KEY_A) == elf.DOWN then
elf.MoveActorLocal (cam, -12.0, 0.0, 0.0)
end

if elf.GetKeyState (elf.KEY_D) == elf.DOWN then
elf.MoveActorLocal (cam, 12.0, 0.0, 0.0)
end

if elf.GetKeyState (elf.KEY_ESC) == elf.PRESSED then
elf.Quit ()
end
end
```

Run and check. If the game does not close when you press ESC, then you can turn off the ALT + F4 combinations or just press F10.
Now let's make the camera respond to mouse turns. To do this, we need to know the difference between the initial angle of rotation of the camera and the angle after the mouse stops when you move it. The elf.GetMouseForce function will help us with this. Assign its value to the variable mf and add the camera rotation code when moving the mouse:

```
elf.HideMouse (true)
elf.LoadScene ("level1.pak")
while elf.Run () == true do
cam = elf.GetSceneActiveCamera (scn)

if elf.GetKeyState (elf.KEY_W) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, -12.0)
end

if elf.GetKeyState (elf.KEY_S) == elf.DOWN then
elf.MoveActorLocal (cam, 0.0, 0.0, 12.0)
end

if elf.GetKeyState (elf.KEY_A) == elf.DOWN then
elf.MoveActorLocal (cam, -12.0, 0.0, 0.0)
end

if elf.GetKeyState (elf.KEY_D) == elf.DOWN then
elf.MoveActorLocal (cam, 12.0, 0.0, 0.0)
end

mf = elf.GetMouseForce ()
elf.RotateActor (cam, 0.0, 0.0, -mf.x * 10.0)
elf.RotateActorLocal (cam, -mf.y * 10.0, 0.0, 0.0)

if elf.GetKeyState (elf.KEY_ESC) == elf.PRESSED then
elf.Quit ()
end
end
```

The mouse and 3D spaces have a different coordinate system, so in the rotation functions we use the negative mf value.
The elf.RotateActor 

```
function (cam, 0.0, 0.0, -mf.x * 10.0)
```

here is responsible for rotating the camera along the Y axis relative to the camera itself. 
Respectively,

```
elf.RotateActorLocal (cam, -mf.y * 10.0, 0.0, 0.0)
```
for a turn in X, again, relative to the camera, this is very important. 
It is also worth noting that in the BlendELF engine all operations with object rotation use degrees, not radians, which is also very important.

Finally, you can start the game. 
You can fly through the level and look around with the mouse, a very good basis for a first-person shooter