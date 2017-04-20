CAMLC = ocamlc
CC = g++
EXEC = prog


CUSTOM = -custom

WITHGL = -cclib -lGL  -cclib -lGLU -cclib -lglut -cclib -lGLEW -cclib -lSDL2 -cclib -lstdc++


LIBCAML = -I /usr/local/lib/caml
LIBSPOC = -I /home/naim/SPOC-121217/Spoc 


OBJSML = display.cmo shader.cmo mesh.cmo texture.cmo

OBJC = display.o shader.o mesh.o texture.o stb_image.o obj_loader.o

OBJS = $(OBJC) $(OBJSML) prog.cmo

all:  $(OBJS)

display.o: 
	$(CC) -c  $(LIBCAML)   src/display.cpp 

shader.o: 
	$(CC) -c  $(LIBCAML)   src/shader.cpp 

mesh.o: 
	$(CC) -c  $(LIBCAML)   src/mesh.cpp 

texture.o: 
	$(CC) -c  $(LIBCAML)   src/texture.cpp 

stb_image.o:
	$(CC) -c $(LIBCAML) src/stb_image.c
	
obj_loader.o:
	$(CC) -c $(LIBCAML) src/obj_loader.cpp
display.cmo:
	$(CAMLC) -c display.ml

shader.cmo:
	$(CAMLC) -c shader.ml

mesh.cmo:
	$(CAMLC) -c mesh.ml

texture.cmo:
	$(CAMLC) -c texture.ml

prog.cmo:
	$(CAMLC) $(CUSTOM) -o $(EXEC) unix.cma $(OBJSML) prog.ml $(OBJC) $(WITHGL)


clean:
	rm -f *.[oa] *.so *.cm[ixoa] *.cmxa *.o prog.exe