THREADS=${1:-"1 2 4 8"}
NCARS=${2:-3000}
TICKS=${3:-4000}
RUNS=${4:-64}

cd "$(dirname "$0")"
make -s parallel_traffic ensemble_traffic || exit 1

echo "== one simulation split across threads: $NCARS cars, $TICKS ticks =="
echo "threads,serial_s,stepParallel_s,runParallel_s,best_speedup,efficiency" > one.csv
for p in $THREADS; do
    line=$(OMP_NUM_THREADS=$p ./parallel_traffic "$NCARS" "$TICKS" | grep '^RESULT')
    echo "$line" | awk '{ printf "%s,%s,%s,%s,%s,%.3f\n", $2, $5, $6, $7, $8, $8 / $2 }' >> one.csv
done
tr "," "\t" < one.csv

echo
echo "== many simulations at once: $RUNS runs of 150 cars, 6000 ticks =="
echo "threads,serial_s,parallel_s,speedup,efficiency" > ensemble.csv
for p in $THREADS; do
    line=$(OMP_NUM_THREADS=$p ./ensemble_traffic 150 6000 "$RUNS" | grep '^RESULT')
    echo "$line" | awk '{ printf "%s,%s,%s,%s,%.3f\n", $2, $6, $7, $8, $8 / $2 }' >> ensemble.csv
done
tr "," "\t" < ensemble.csv
