#
# Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#  Abstract: a makefile for 'Post-Browser Interface #2' project

CPPFLAGS=-D__FORUNIX__   -D__FORGCC__      -I./include   
#
# -D__FORx86__ -- not needed as long we no assembly is deployed
#
# -D__FORMOTIF__  -- outdate framework to deploy XWindow interface 
#
# -g debug symbols (to explore dumped core, for instance)
#
# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

LDFLAGS=-lX11 -lXt -lXm -lc -lm


OBJS=./source/g3d3ds.o ./source/g3dasc.o ./source/G3dcam.o ./source/G3ddraw.o ./source/g3dfile.o ./source/G3dgem.o ./source/G3dgeo.o ./source/G3dlight.o ./source/G3dmath.o ./source/G3dmatrl.o ./source/G3dobj.o ./source/G3dpobj.o ./source/g3dpoly.o ./source/G3dshape.o ./source/G3dsystm.o ./source/g3dworld.o ./source/gfxadst.o ./source/gfxanim.o ./source/gfxbmp.o ./source/Gfxcltbl.o ./source/gfxddraw.o ./source/gfxdev.o ./source/gfxdisp.o ./source/gfxdpmi.o ./source/gfxdrv.o ./source/gfxerror.o ./source/gfxfiles.o ./source/gfxflic.o ./source/gfxgdi.o ./source/gfxgif.o ./source/gfxgpi.o ./source/gfxgraf.o ./source/gfxifile.o ./source/gfximage.o ./source/gfxinput.o ./source/gfxllist.o ./source/Gfxmodes.o ./source/gfxobj.o ./source/Gfxpal.o ./source/gfxpcx.o ./source/gfxsddrv.o ./source/gfxsound.o ./source/gfxsys.o ./source/gfxtimer.o ./source/gfxtypes.o ./source/Gfxvesa.o ./source/Gfxvga.o ./source/gfxwinsd.o ./source/gfxxwin.o ./source/guiapp.o ./source/guichild.o ./source/guicmdlg.o ./source/guidlg.o ./source/guifloat.o ./source/guiframe.o ./source/guimdi.o ./source/guimenu.o ./source/guiobj.o ./source/guitbar.o ./source/guiwin.o       ./source/lg3dmatg.o 	 ./source/lg3drasg.o 	 ./source/lgfxdibg.o 	 ./source/lgfxvgag.o 	 ./source/lgfxvsag.o 	./source/mpgui.o                       ./source/3ddos.o

# Not cross-compiling till instance for <Linux/i585> platform is ready
PREFIX=

CC=$(PREFIX)gcc

CPP=$(PREFIX)g++

LD=$(PREFIX)g++

TARGET=_main

all:	$(OBJS)  $(TARGET)

%.o: %.cpp 
	$(CPP) $(CPPFLAGS)  -c -o $@ $<

$(TARGET): $(OBJS)
	$(CPP)  $(OBJS)   -o $@   $(LDFLAGS)


#GRBGD=./autom4te.cache  ./lintstats
#GRBG += Makefile configure

GRBG=./source/*.o ./source/*~    ./*~   $(TARGET)      config.status config.log

clean:
	rm $(GRBG) #; rm -r -v $(GRBGD)
