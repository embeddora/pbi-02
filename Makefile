#
#  Abstract: 

export CC=gcc

BUILD_CURL=

# Well, let's start to...

CFLAGS=-D__FORUNIX__   -D__FORGCC__        -I./include   
# -D__FORx86__
# -D__FORMOTIF__  

LDFLAGS=

# -g debug symbols (to explore dumped core, for instance)

# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

GRBG=source/*.o *~   config.status config.log

#GRBG += Makefile configure

GRBGD=./autom4te.cache  ./lintstats



OBJS=./source/g3d3ds.o ./source/g3dasc.o ./source/G3dcam.o ./source/G3ddraw.o ./source/g3dfile.o ./source/G3dgem.o ./source/G3dgeo.o ./source/G3dlight.o ./source/G3dmath.o ./source/G3dmatrl.o ./source/G3dobj.o ./source/G3dpobj.o ./source/g3dpoly.o ./source/G3dshape.o ./source/G3dsystm.o ./source/g3dworld.o ./source/gfxadst.o ./source/gfxanim.o ./source/gfxbmp.o ./source/Gfxcltbl.o ./source/gfxddraw.o ./source/gfxdev.o ./source/gfxdisp.o ./source/gfxdpmi.o ./source/gfxdrv.o ./source/gfxerror.o ./source/gfxfiles.o ./source/gfxflic.o ./source/gfxgdi.o ./source/gfxgif.o ./source/gfxgpi.o ./source/gfxgraf.o ./source/gfxifile.o ./source/gfximage.o ./source/gfxinput.o ./source/gfxllist.o ./source/Gfxmodes.o ./source/gfxobj.o ./source/Gfxpal.o ./source/gfxpcx.o ./source/gfxsddrv.o ./source/gfxsound.o ./source/gfxsys.o ./source/gfxtimer.o ./source/gfxtypes.o ./source/Gfxvesa.o ./source/Gfxvga.o ./source/gfxwinsd.o ./source/gfxxwin.o ./source/guiapp.o ./source/guichild.o ./source/guicmdlg.o ./source/guidlg.o ./source/guifloat.o ./source/guiframe.o ./source/guimdi.o ./source/guimenu.o ./source/guiobj.o ./source/guitbar.o ./source/guiwin.o       ./source/lg3dmatg.o 	 ./source/lg3drasg.o 	 ./source/lgfxdibg.o 	 ./source/lgfxvgag.o 	 ./source/lgfxvsag.o 	./source/mpgui.o                       ./source/3ddos.o
# ./source/3dwin.o   
## ./source/dlgs.o


CC=$(PREFIX)gcc

CPP=$(PREFIX)g++

LD=$(PREFIX)g++

all:	$(OBJS)  _main

%.o: %.cpp 
	$(CPP) $(CFLAGS)  -c -o $@ $<

_main: $(OBJS)
	g++ $(LDLAGS)   $(OBJS)  -lcurl -lX11 -lXt -lXm -lc -lm -o $@


clean:
	rm $(GRBG) ; rm -r -v $(GRBGD)
