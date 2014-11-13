GzFramework
===========
graphics renderer

/* YASH ABOOJ

   README */

/******************************************************************************
COMPILATION

MS Visual Studio:	Version 2013
Build Mode: 		Debug(x32)
GzFramework is used for the program.
Build the project and use the GUI options to Render, Rotate, Translate and Scale
the tea-pot.

******************************************************************************/
/******************************************************************************
ANTI-ALIASING METHOD

Jittered Sub-Sampling
- six sample renders are generated according to the specified (dx, dy) offsets
- the offsets specified in the AAFilter are subtracted from the triangle vertices
- the six renders are then weighted and combined to produce the final render

TEXTURE MAPPING METHOD

The current texture mapping method is set to image mapping with Phong vertex 
normals interpolation shading.

To change the Texture Mapping Method - 
1. Open Application6.cpp
2. Search for macro that sets 'valueListShader[5]'
3. Change (tex_fun) to (ptex_fun) for procedural texturing

Note: Effect of procedural texturing is prominent when the default camera is used.
Julia Set method is used to implement procedural texturing.

To change the shading method -
1. Open Application6.cpp
2. Serch for 'interpStyle'
3. Change the GZ_NORMALS flag to GZ_COLOR to render using Gouraud shading and to 
GZ_NONE to render using flat shading.

******************************************************************************/
