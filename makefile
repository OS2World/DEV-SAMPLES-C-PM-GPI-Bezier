# nmake makefile
# 
# Tools used:
#  Compile::Resource Compiler
#  Compile::GNU C
#  Make: nmake
all : bezier.exe

bezier.exe : bezier.obj bezier.def
	gcc -Zomf bezier.obj bezier.def -o bezier.exe

bezier.obj : bezier.c 
	gcc -Wall -Zomf -c -O2 bezier.c -o bezier.obj

clean :
	rm -rf *exe *RES *obj