from subprocess import call

call(["gcc","-g","-O3", "-Wall", "-o", "segerast" ,"segerast.c", "-lm"])
call(["gcc","-g","-O3", "-Wall", "-o", "erast" ,"erast.c", "-lm"])
call(["mpicc","-g","-O3", "-Wall", "-o", "parasegerast" ,"parasegerast.c", "-lm"])