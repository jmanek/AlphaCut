# AlphaCut
### Clip a triangle mesh with transparency maps

AlphaCut allows you to perform a boolean difference on a mesh using a texture's alpha channel as the cutting object.  

#### Usage

Currently there is only support for .objs

```
make
./alphacut [filepath]
```

A new .obj is then written into the same directory 

#### Dependencies
[CGAL] (http://www.cgal.org/) is used for some exact operations (also why this library is GPL).  You will also need X11, pthread, and libpng


#### Example

Here is a plane with a transparent texture mapped to it:

![](https://github.com/jmanek/AlphaCut/blob/master/Tests/original.png)

After running it through AlphaCut, all transparent sections are removed and only the opaque sections remain:

![](https://github.com/jmanek/AlphaCut/blob/master/Tests/cut1.png)

it does this by creating geometry based on the pixels of the texture map:

![](https://github.com/jmanek/AlphaCut/blob/master/Tests/cut2.png)

While the output is currently very large, because the new geometry is planar relative to its parent triangle, it reduces very well.  Eventually it will reduce polygon count automatically.  For now Meshlab's simplification modifiers work very well.

