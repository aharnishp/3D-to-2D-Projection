# 3D Model to 2D Projection
This program is only a test program for projecting 3D model in Raspberry Pi Pico using only CPU.  

Currently only supports Orthographic Projection of 3D edges array to 2D raster array.

Future plans of adding a perspective camera and projecting the model to it with support for faces.


## Usage
Insert an obj file in the repo folder, than modify the filename in the ```extract_from_obj.py``` python script to new .obj filename.

Then run the python file. New ```model.h``` file will be generated for the new model containing only node and edge information. 
(If necessary, you can modify the scale argument in the project2D function called in ```drawLineTest.c``` file to fit in the drawing region.)

Then compile the ```drawLineTest.c``` file which includes the new model.h file. 

Run the ```drawLineTest.c``` to view the rasterised array in the terminal. (You can zoom out character in terminal with shortcuts like "ctrl" + "shift" + "-" (linux) or "cmd" + "-" (mac))