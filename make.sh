gcc biblioteca.c -c;
gcc build_BA.c biblioteca.o -o build_BA;
gcc sznajd_inflow.c biblioteca.o -o sznajd_inflow;
gcc sznajd_outflow.c biblioteca.o -o sznajd_outflow;
gcc voter.c biblioteca.o -o voter;
