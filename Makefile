
display: ppm.o display_main.o
	mpic++ display.o ppm.o -o display -lgdi32 -g

ppm.o: ppm.c *.h *.hh
	mpic++ -c ppm.c -g

wireworld.o: wireworld.c *.h *.hh
	mpic++ -c wireworld.c -g

display.o: display.cpp *.h *.hh
	mpic++ -c display.cpp -g

simulation.o: simulation.c *.h *.hh
	mpic++ -c simulation.c -g

wireworld: wireworld.o ppm.o display.o
	mpic++ wireworld.o ppm.o display.o -o wireworld -lgdi32 -g

simulation: wireworld.o ppm.o display.o simulation.o
	mpic++ wireworld.o ppm.o display.o simulation.o -o simulation -lgdi32 -g

measure.o: measure.c *.h
	mpic++ -c measure.c -g

measure: measure.o wireworld.o ppm.o
	mpic++ wireworld.o ppm.o measure.o -o measure -g

clean:
	rm *.o *.exe

