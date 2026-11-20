How to compile and run traffic simulator:
- Setup: The simulation requires no further setup on Linux or the cluster. 
        MAC OS: Start by running `brew install libomp raylib` on a Mac.- Build: The raylib visualizer is built by `make renderer`; serial traffic, parallel traffic, ensemble traffic, and test traffic are built by `make`.

- Serial run: `./serial_traffic [numCars] [ticks] [seed] [printEvery]`. For instance, `./serial_traffic 60 6000`. Ten simulated minutes are represented by 6000 ticks, each of which is equal to 0.1 seconds.

- One thread simulation: `OMP_NUM_THREADS=4 ./parallel_traffic [numCars] [ticks] [seed]` multiplies the serial and parallel versions and verifies that they coincide.

- Multiple simulations simultaneously: `OMP_NUM_THREADS=4 ./ensemble_traffic [numCars] [ticks] [runs] [seed]` does separate simulations with various seeds, one after the other, then on threads, and compares the times.

- Tests: `make check` executes `test_traffic` and outputs PASS or FAIL for every check.

- Speedup tables: The results are written to `one.csv` and `ensemble.csv` by `./bench.sh "1 2 4 8 16" 3000 4000 64`.Visualizer: `./renderer [numCars] [seed]` launches a window. Cars are represented by dots that are colored according to the exit they are going to.