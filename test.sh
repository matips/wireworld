#PBS -l walltime=00:20:00
#PBS -l nodes=1:ppn=12

#module load openmpi
#module load libs/boost/1.52.0
#module load plgrid/tools/mpich/3.0.4

#pip install --user mpi4py

module load libs/boost/1.52.0 > /dev/null 2>&1
pip install --user mpi4py > /dev/null 2>&1
module load mvapich2 > /dev/null 2>&1
module load mpiexec > /dev/null 2>&1
module add plgrid/tools/python/2.7.5 > /dev/null 2>&1


NPROCS="1 2 3 4 5 6 7 8 9 10 11 12"
SIZES="10000 35000 71000 142000"

mkdir -p ./out
for SIZE in $SIZES
do	
	touch ./out/out_$SIZE.txt
	echo "world_size; width; height; steps; time [s]" > ./out/out_$SIZE.txt

	for NPROC in $NPROCS
	do
		mpirun -np $NPROC measure.exe 1000 $SIZE 1000 >> ./out/out_$SIZE.txt
	done
done
         