emcc -O3 %1 -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1  -s AGGRESSIVE_VARIABLE_ELIMINATION=1 