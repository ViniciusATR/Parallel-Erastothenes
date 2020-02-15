import sys
from subprocess import call

n = [1000000,10000000,100000000,1000000000,10000000000]
it = int(sys.argv[1])


for i in n:
    for j in range(it):
        call(["./segerast", str(i)])

num_proc = [1,2,4,8,16]


for i in n:
    for j in num_proc:
        print("Testando com {j} processadores")
        for k in range(it):
            call(["mpiexec","--oversubscribe","-n",str(j),"parasegerast",str(i)])
