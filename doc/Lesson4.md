# Lesson 4: Physics and Collisions

In this lesson, we will teach you how to interact with each other at our game level.

## Step 1: Object Names

At the moment, we do not use any objects except the camera. 
First, let's take a couple and set physical settings for them.

All objects at the level have their own name, and it is through it that we will refer to them. 
The image below shows the names we will use:

![Lesson_4_1](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson4_1.jpg)

Why is the ball called Cube.002? This is a long story, and so far it’s absolutely not important ...

So now we know the names of two objects. They will interact with each other in the future.

## Step 2: Physical Primitives and Mass

Let's get back to the code from the first lesson:

```
scn = elf.LoadScene ("level1.pak")
while elf.Run () == true do
end
```

We need to change the code, add two lines after elf.LoadScene. 
If we add them to this line, we get an error, because the scn variable will not contain anything.
Finally, we turn to our two objects and write them into the variables ent1 and ent2:

```
scn = elf.LoadScene ("level1.pak")

ent1 = elf.GetEntityByName (scn, "Plane")
ent2 = elf.GetEntityByName (scn, "Cube.002")

while elf.Run () == true do
end
```

The elf.GetEntityByName function takes from the scene, which is the first argument, objects under the desired name, which, respectively, is the second argument to this function. 
So we “captured” our sphere and gender and can establish physics for them:

```
scn = elf.LoadScene ("level1.pak")

ent1 = elf.GetEntityByName (scn, "Plane")
ent2 = elf.GetEntityByName (scn, "Cube.002")

elf.SetActorPhysics (ent1, elf.MESH, 0.0)
elf.SetActorPhysics (ent2, elf.SPHERE, 1.0)

while elf.Run () == true do
end
```

The first argument to the SetActorPhysics function is the object that we “teach” physical laws, the second argument is the type of physical primitive. 
And the third is the mass. In BlendElf, all static objects, such as gender, have zero mass.

What is a physical primitive?

In BlendElf, every object obeying physics should have one of the primitives available in the engine, in fact, this is the definition of the shape of our object.

Here is a list of available primitives:
* elf.BOX - ordinary box \ cube
* elf.SPHERE - ball \ sphere
* elf.MESH is a model that collides with its polygons. Best suited for static objects like floor \ walls.
* elf.CAPSULE_X - Capsule (ellipse, oval) with ends along the X axis
* elf.CAPSULE_Y - Capsule (ellipse, oval) with ends along the Y axis
* elf.CAPSULE_Z - Capsule (ellipse, oval) with ends along the Z axis
* elf.CONE_X - Cone with an angle along the X axis
* elf.CONE_Y - Cone with an angle along the Y axis
* elf.CONE_Z - Cone with an angle along the Z axis

The dimensions of the primitive are usually determined automatically (for objects obeying physics, of course), so you can not worry about this. 
But just in case, you yourself can determine the size of your body with the elf.SetActorBoundingLengths function.
For elf.MESH size calculation is not required, as information about it has already been loaded into BlendElf and can be safely used in the physics engine.

Launch the game. 
Congratulations, now your ball falls down and rolls across the floor.

## Step 3: Add Interests

The falling ball is not very interesting, let's add some initial speed to it.

```
scn = elf.LoadScene ("level1.pak")

ent1 = elf.GetEntityByName (scn, "Plane")
ent2 = elf.GetEntityByName (scn, "Cube.002")

elf.SetActorPhysics (ent1, elf.MESH, 0.0)
elf.SetActorPhysics (ent2, elf.SPHERE, 1.0)
elf.SetActorLinearVelocity (ent2, 0.0, 0.0, 6.0)

while elf.Run () == true do
end
```

The first argument of the new elf.SetActorLinearVelocity function is the object we want to speed up, the other three are the speed in a certain direction: along x, y, z.