
expr1:
	bison -d expr1.y
	gcc -o expr1 expr1.tab.c

expr2:
	bison expr2.y
	gcc -o expr2 expr2.tab.c

clean:
	-rm -f *.c *.h *.out expr1 expr2
