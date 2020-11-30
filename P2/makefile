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

main: main.o minimiza.o list.o extrafunctions_int.o clases_de_estados.o queue_fp.o lib_afnd_64.a 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS) minimiza.o list.o extrafunctions_int.o clases_de_estados.o queue_fp.o

main.o: main.c  
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(CFLAGS)


minimiza.o: minimiza.c list.c clases_de_estados.c queue_fp.c
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) -c -o $@ $< $(LDFLAGS) $(LDLIBS) $(LIBS)

clases_de_estados.o: clases_de_estados.c list.c extrafunctions_int.c
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

queue_fp.o: queue_fp.c 
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c -o $@ $< 



