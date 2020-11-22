CC = -gcc
CFLAGS=-Wall
IFLAGS=-I./
LDFLAGS = -L./
LDLIBS=-l_afnd_64
LIBS = -lm -pthread

all : main

.PHONY: clean val
clean:
	 rm *.o  main determinista.dot

val : 
	@valgrind --leak-check=full ./main

main: main.o transforma.o estado.o transicion.o list.o extrafunctions_int.o lib_afnd_64.a 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS) transforma.o estado.o transicion.o list.o extrafunctions_int.o

main.o: main.c  
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(CFLAGS)


transforma.o: transforma.c list.c estado.c transicion.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS)

estado.o: estado.c list.c extrafunctions_int.c
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS)


transicion.o: transicion.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS)

extrafunctions_int.o: extrafunctions_int.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c -o $@ $< 

list.o: list.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c -o $@ $< 



