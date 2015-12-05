WIP level parallelism based algorithm for the implementation of `make_heap` and `is_heap`
within HPX parallel algorithms library.

### Usage
-----------------
`./heap --vector_size=<size>`

example output: 

```
> ./heap --vector_size=100000000
Distance of tree: 100000000
Height: 26
Running parallel make heap...
Running sequential make heap...
---------------------------------------------
Results:
Parallel: 4.76452
Sequential: 7.0283
```

### Building
-----------------
Repository depends on HPX and a C++11 compliant compiler.

#### Linux
1. clone the master HPXLA git repository
  
   `git clone http://github.com/STEllAR-GROUP/hpxla.git`

2. create a build directory. HPXLA should be an out-of-tree build
  
   ```
   mkdir hpxla_build
   cd hpxla_build
   ```

3. Invoke CMake, note that you'll likely need to specify the same compiler used to build HPX
  
   ```
   cmake -DCMAKE_CXX_COMPILER=/usr/bin/compiler \
          -DHPX_ROOT=/path/to/hpxbuild           \
          /path/to/hpxla/source
   ```

   e.g.

   ```
   cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++  \
          -DHPX_ROOT=/home/dev/hpx_build     \
          ../hpxla
   ```

4. Make to build all targets

   `make`


