OPENGL_LIBS = -lglut -lGL -lGLU

CXX = $(shell wx-config --cxx)

SRC = logsim.cc names.cc scanner.cc network.cc parser.cc monitor.cc devices.cc userint.cc symbol.cc gui.cc #guitest.cc

L_OBJECTS = logsim.o names.o scanner.o network.o parser.o monitor.o devices.o userint.o  symbol.o gui.o

G_OBJECTS = guitest.o names.o network.o monitor.o devices.o gui.o

# implementation

.SUFFIXES:	.o .cc

.cc.o :
	$(CXX) -c `wx-config --cxxflags` -g -o $@ $<

all:    logsim 
	#guitest

logsim:	$(L_OBJECTS)
	$(CXX) -o logsim $(L_OBJECTS) `wx-config --libs --gl_libs` $(OPENGL_LIBS)

#guitest: $(G_OBJECTS)
	# $(CXX) -o guitest $(G_OBJECTS) `wx-config --libs --gl_libs` $(OPENGL_LIBS)

clean: 
	rm -f *.o logsim #guitest

depend:
	makedepend $(SRC)

# DO NOT DELETE

logsim.o: logsim.h names.h devices.h network.h monitor.h parser.h scanner.h
logsim.o: symbol.h userint.h gui.h /usr/include/GL/glut.h
logsim.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
logsim.o: /usr/include/GL/glext.h /usr/include/inttypes.h
logsim.o: /usr/include/features.h /usr/include/sys/cdefs.h
logsim.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
logsim.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
logsim.o: /usr/include/bits/wchar.h /usr/include/GL/glu.h
names.o: names.h
scanner.o: scanner.h symbol.h names.h
network.o: network.h names.h
parser.o: parser.h names.h scanner.h symbol.h network.h devices.h monitor.h
monitor.o: monitor.h names.h network.h devices.h
devices.o: devices.h names.h network.h
userint.o: userint.h names.h network.h devices.h monitor.h
gui.o: gui.h names.h devices.h network.h monitor.h /usr/include/GL/glut.h
gui.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
gui.o: /usr/include/GL/glext.h /usr/include/inttypes.h
gui.o: /usr/include/features.h /usr/include/sys/cdefs.h
gui.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
gui.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
gui.o: /usr/include/bits/wchar.h /usr/include/GL/glu.h wx_icon.xpm
#guitest.o: guitest.h names.h devices.h network.h monitor.h gui.h
#guitest.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
#guitest.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
#guitest.o: /usr/include/inttypes.h /usr/include/features.h
#guitest.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
#guitest.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
#guitest.o: /usr/include/stdint.h /usr/include/bits/wchar.h
#guitest.o: /usr/include/GL/glu.h
symbol.o: symbol.h
