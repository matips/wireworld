#PBS -l walltime=05:00:00
#PBS -l nodes=2:ppn=12

#module load openmpi
#module load libs/boost/1.52.0
#module load plgrid/tools/mpich/3.0.4

#pip install --user mpi4py

module load libs/boost/1.52.0 > /dev/null 2>&1
pip install --user mpi4py > /dev/null 2>&1
module load mvapich2 > /dev/null 2>&1
module load mpiexec > /dev/null 2>&1
module add plgrid/tools/python/2.7.5 > /dev/null 2>&1


NPROCS="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24"
SIZES="1000 10000 35000 71000 142000"

SIZES=( 1000 10000 35000 71000 142000 )
STEPS=( 1000 500   300   200   100 )

mkdir -p ./out
for idx in "${!SIZES[@]}"
do
	SIZE=${SIZES[$idx]}
	STEP=${STEPS[$idx]}

	touch ./out/out_$SIZE.txt
	echo "world_size; width; height; steps; time [s]" > ./out/out_$SIZE.txt

	for NPROC in $NPROCS
	do
		mpirun -np $NPROC measure.exe 1000 $SIZE $STEP >> ./out/out_$SIZE.txt
	done
done

