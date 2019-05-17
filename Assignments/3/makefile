CC=gcc
CFLAGS=-Wall -g
LIBS=-pthread

buildPg1: Assignment3_template_Prg_1.c
	$(CC) $(CFLAGS) Assignment3_template_Prg_1.c -o Assignment3_template_Prg_1.out $(LIBS)

buildPg2: Assignment3_template_Prg_2.c
	$(CC) $(CFLAGS) Assignment3_template_Prg_2.c -o Assignment3_template_Prg_2.out $(LIBS)

runPg1: buildPg1
	./Assignment3_template_Prg_1.out

runPg2: buildPg2
	./Assignment3_template_Prg_2.out

debugPg1: buildPg1
	gdb ./Assignment3_template_Prg_1.out

debugPg2: buildPg2
	gdb ./Assignment3_template_Prg_2.out

