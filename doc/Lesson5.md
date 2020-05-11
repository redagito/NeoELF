# Lesson 5: Export from BlendElf

In this tutorial, we will learn how to export scenes from Blender to a PAK format that BlendElf understands. 
The lesson is not aimed at creating the scene itself in Blender, if you want to learn this - google to help you. 
I think many people know how to use a Blender, so we will not devote much time to this. 
Important: the lesson uses the 2.49 version of Blender. 
You can find the exporter for version 2.5 yourself.

## Step 1: Required Resources

Everything you need for export is included in the BlendElf Development Kit. 
It includes both an engine and scripts for export. 
If you have not downloaded it yet, then you are welcome to BlendElf.com

## Step 2: Find Tools for Export

They are located in the tools folder, namely we need the blender_pak_exporter_fast.py script. 
Advanced Blender users can simply transfer the script to the Blender script folder so that it appears in the menu. 
However I will describe the procedure for running scripts for beginners, as often even looking for a folder of Blender scripts puts them in a stupor.

## Step 3: running the script in Blender

The first thing we should do is open the Text Editor. 
It can be found in the Window Type menu:

![Lesson_5_1](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_1.jpg)

Now in the text editor we have to open our script blender_pak_exporter_fast.py:

![Lesson_5_2](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_2.jpg)

... and we can run it:

![Lesson_5_3](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_3.jpg)

A file browser will open, through which you can save your PAK to any folder. 
Important: Initially, the file will have the extension .blend, but you will need to change it to .pak so that the elf.LoadScene function can open it.

![Lesson_5_4](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_4.jpg)

## Step 4: Configure Physics and Collisions for BlendElf

In Blender 2.49, there is no access to the physical properties of objects, but we can configure their properties.

First find the Logic buttons in Blender:

![Lesson_5_5](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_5.jpg)

See the big Add Property button? 
I think it’s not worth saying that with its help you can set the properties of the selected object. 
Let's set the form:

![Lesson_5_6](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_6.jpg)

We set the string variable shape and the variable BOX. 
The exporter recognizes this object as elf.BOX. 
Our object has no mass (mass will be equal to 0.0), and therefore it will be static. 

In the same way, we can set the mass and some other important properties:
* shape - A variable of type String. Available variables: [BOX | SPHERE | MESH | CAPSULE_X | CAPSULE_Y | CAPSULE_Z | CONE_X | CONE_Y | CONE_Z]
* mass - A variable of type Float, positive.
* lin_damp - A variable of type Float, positive.
* ang_damp - A variable of type Float, positive.

## Step 5: Normal maps and displacement maps.

The following image shows the texture settings, which include normal maps (original normal mapping) and displacement maps (original displacement mapping):

![Lesson_5_7](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_7.jpg)

If you need a reflection map (original specular map), then just enable the Spec option
Important: All reflections in BlendElf go through the reflection map, unlike the Blender, which by default combines normal reflections with the reflection map.

## Step 6: Scripting Objects Through Blender

To add a new script in the Text Editor, select Text -> New. 
Important: scripts MUST have the extension .lua, otherwise the exporter simply does not recognize it.

![Lesson_5_8](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_8.jpg)

Now you need to give the script a name, for example myscript.lua:

![Lesson_5_9](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_9.jpg)

You also need to create a String property called script:

![Lesson_5_10](https://github.com/redagito/NeoELF/blob/master/doc/images/lesson5_10.jpg)

This is all you need to know about scripts. 
If you did everything correctly, then the script will be exported to BlendElf, doing what it should do

## Step 7: Armatures

When exporting rebar (original armature) from Blender to BlendELF, you must take into account the position and position of the rebar. 
It must be exactly the same as after the deformation of the reinforcement.
Otherwise, the animation will not look right.