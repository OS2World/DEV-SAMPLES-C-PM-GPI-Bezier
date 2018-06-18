

#------------------
# BEZIER make file
#------------------

bezier.obj : bezier.c
     cl -c -G2ws -W3 bezier.c

bezier.exe : bezier.obj bezier.def
     link bezier, /align:16, NUL, os2, bezier

