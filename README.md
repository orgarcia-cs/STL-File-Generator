Accepts one or two arguments.

compiled using gcc with the usual arguments; most .h files can be removed from include header above main they're just the generic stuff I add everwhere but this process is so simple it doesn't need many .h includes.

a.out <somefile.stl> <anotherfile.stl>

Will read in somefile.stl and produce output describing the super simple header file for STL.

If a second argument is given e.g. outputfile.stl it will write out a simple cube as an STL file.
To verify I opened file in Blender.
 
Next steps: Write as a class, then develop algorithm, if possible, to find surface triangles and flatten facets to a 2D projection - this will prepare for converting an STL file to a surface mesh OBJ file
