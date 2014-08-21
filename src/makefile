#--release
CC= cl.exe /nologo /G6 /ML /GX /O2 /Ob2 /W2 /Ox /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c 

#--openPTC release
NAME = main
LIBS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib c:\openPTC\library\visual\ptc.lib

#--release
LINK= link.exe -map -pdb:none -out:../$(NAME).exe -subsystem:windows /incremental:no

DEF =-DDEBUG -DSTRICT

OBJ = main_ptc.obj\
	main_common.obj\
	trace.obj\
	bsp.obj\
	object.obj\
	color.obj\
	ray.obj\
	camera.obj\
	world.obj\
	lighting.obj\
	finish.obj\
	texture.obj\
	interior.obj\
	noise.obj\
	turbulence.obj\
	colormap.obj\
	globals.obj\
	intersection.obj\
	parser\scanner.obj\
	parser\parser.obj\
	renderers\renderer.obj\
	renderers\bilinear.obj\
	renderers\corner.obj\
	renderers\grid.obj\
	renderers\rgss.obj\
	renderers\diamond.obj\
	renderers\flipquad.obj\
	renderers\quincunx.obj\
	animators\animator.obj\
	animators\mover.obj\
	animators\add.obj\
	animators\bezier.obj\
	animators\bounce.obj\
	animators\circle.obj\
	animators\linearf.obj\
	animators\lissajou.obj\
	bitmap\bitmap.obj\
	bitmap\tga.obj\
	bitmap\pcx.obj\
	bitmap\bmp.obj\
	math\vector2.obj\
	math\vector3.obj\
	math\vector4.obj\
	math\solver.obj\
	math\matrix4.obj\
	math\matrix3.obj\
	math\cycloidal.obj\
	lights\light.obj\
	lights\dot.obj\
	boundings\bounding.obj\
	boundings\aabb.obj\
	primitives\primitive.obj\
	primitives\sphere.obj\
	primitives\box.obj\
	primitives\plane.obj\
	primitives\cylinder.obj\
	primitives\metaobjects.obj\
	primitives\capsule.obj\
	primitives\torus.obj\
	primitives\transformed.obj\
	primitives\csg.obj\
	primitives\quadric.obj\
	primitives\heart.obj\
	ambients\ambient.obj\
	ambients\solid.obj\
	ambients\hemispherical.obj\
	pigments\pigment.obj\
	pigments\solid.obj\
	pigments\noisy.obj\
	pigments\granite.obj\
	pigments\mandel.obj\
	pigments\marble.obj\
	pigments\wood.obj\
	pigments\checker.obj\
	pigments\cubemap.obj\
	pigments\cylindrical.obj\
	pigments\spherical.obj\
	pigments\skymap.obj\
	pigments\planar.obj\
	pigments\torusmap.obj\
	atmospherics\atmospheric.obj\
	atmospherics\fog.obj\
	atmospherics\ground.obj\
	text\basics.obj\
	fonts\font.obj\
	poly3d\wire.obj\
	poly3d\rendersimple.obj\
	poly3d\camerasimple.obj

.cpp.obj:
        $(CC) /Fo$*.obj /Fl$*.lst $< 

goal: $(NAME).exe

$(NAME).exe: $(OBJ) makefile
	$(LINK) $(OBJ) $(LIBS)

clean:
        -del ..\$(NAME).exe
	-del *.*~
	-del *.lst
        -del *.res
        -del *.err
        -del *.obj
        -del *.map
        -del *.sym
        -del *.cod
        -del *.pdb
        -del *.idb
        -del *.pch
	-del math\*.*~
	-del math\*.lst
        -del math\*.res
        -del math\*.err
        -del math\*.obj
        -del math\*.map
        -del math\*.sym
        -del math\*.cod
        -del math\*.pdb
	-del types\*.*~
	-del types\*.lst
        -del types\*.res
        -del types\*.err
        -del types\*.obj
        -del types\*.map
        -del types\*.sym
        -del types\*.cod
        -del types\*.pdb
	-del fonts\*.*~
	-del fonts\*.lst
        -del fonts\*.res
        -del fonts\*.err
        -del fonts\*.obj
        -del fonts\*.map
        -del fonts\*.sym
        -del fonts\*.cod
        -del fonts\*.pdb
	-del boundings\*.*~
	-del boundings\*.lst
        -del boundings\*.res
        -del boundings\*.err
        -del boundings\*.obj
        -del boundings\*.map
        -del boundings\*.sym
        -del boundings\*.cod
        -del boundings\*.pdb
	-del primitives\*.*~
	-del primitives\*.lst
        -del primitives\*.res
        -del primitives\*.err
        -del primitives\*.obj
        -del primitives\*.map
        -del primitives\*.sym
        -del primitives\*.cod
        -del primitives\*.pdb
	-del lights\*.*~
	-del lights\*.lst
        -del lights\*.res
        -del lights\*.err
        -del lights\*.obj
        -del lights\*.map
        -del lights\*.sym
        -del lights\*.cod
        -del lights\*.pdb
	-del text\*.*~
	-del text\*.lst
        -del text\*.res
        -del text\*.err
        -del text\*.obj
        -del text\*.map
        -del text\*.sym
        -del text\*.cod
        -del text\*.pdb
	-del poly3d\*.*~
	-del poly3d\*.lst
        -del poly3d\*.res
        -del poly3d\*.err
        -del poly3d\*.obj
        -del poly3d\*.map
        -del poly3d\*.sym
        -del poly3d\*.cod
        -del poly3d\*.pdb
	-del ambients\*.*~
	-del ambients\*.lst
        -del ambients\*.res
        -del ambients\*.err
        -del ambients\*.obj
        -del ambients\*.map
        -del ambients\*.sym
        -del ambients\*.cod
        -del ambients\*.pdb
	-del pigments\*.*~
	-del pigments\*.lst
        -del pigments\*.res
        -del pigments\*.err
        -del pigments\*.obj
        -del pigments\*.map
        -del pigments\*.sym
        -del pigments\*.cod
        -del pigments\*.pdb
	-del bitmap\*.*~
	-del bitmap\*.lst
        -del bitmap\*.res
        -del bitmap\*.err
        -del bitmap\*.obj
        -del bitmap\*.map
        -del bitmap\*.sym
        -del bitmap\*.cod
        -del bitmap\*.pdb
	-del atmospherics\*.*~
	-del atmospherics\*.lst
        -del atmospherics\*.res
        -del atmospherics\*.err
        -del atmospherics\*.obj
        -del atmospherics\*.map
        -del atmospherics\*.sym
        -del atmospherics\*.cod
        -del atmospherics\*.pdb
	-del renderers\*.*~
	-del renderers\*.lst
        -del renderers\*.res
        -del renderers\*.err
        -del renderers\*.obj
        -del renderers\*.map
        -del renderers\*.sym
        -del renderers\*.cod
        -del renderers\*.pdb
	-del animators\*.*~
	-del animators\*.lst
        -del animators\*.res
        -del animators\*.err
        -del animators\*.obj
        -del animators\*.map
        -del animators\*.sym
        -del animators\*.cod
        -del animators\*.pdb
	-del parser\*.*~
	-del parser\*.lst
        -del parser\*.res
        -del parser\*.err
        -del parser\*.obj
        -del parser\*.map
        -del parser\*.sym
        -del parser\*.cod
        -del parser\*.pdb
	-del partitions\*.*~
	-del partitions\*.lst
        -del partitions\*.res
        -del partitions\*.err
        -del partitions\*.obj
        -del partitions\*.map
        -del partitions\*.sym
        -del partitions\*.cod
        -del partitions\*.pdb
	-del turbulences\*.*~
	-del turbulences\*.lst
        -del turbulences\*.res
        -del turbulences\*.err
        -del turbulences\*.obj
        -del turbulences\*.map
        -del turbulences\*.sym
        -del turbulences\*.cod
        -del turbulences\*.pdb
	-del normals\*.*~
	-del normals\*.lst
        -del normals\*.res
        -del normals\*.err
        -del normals\*.obj
        -del normals\*.map
        -del normals\*.sym
        -del normals\*.cod
        -del normals\*.pdb
