#PBS -l walltime=00:20:00
#PBS -l nodes=1:ppn=12

NPROCS="1 2 3 4 5 6 7 8 9 10 11 12"
SIZES="100 1000 10000 35000 71000 142000"

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
