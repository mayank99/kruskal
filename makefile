OBJS = edge.o vertex.o set.o rbt.o gst.o bst.o tnode.o queue.o cda.o integer.o real.o scanner.o 
MAIN = da.o kruskal $(OBJS)
OOPTS = -Wall -Wextra -g -std=c99 -c
LOPTS = -Wall -Wextra -g -std=c99
TESTDIR = .testing/
TESTS = $(sort $(basename $(notdir $(wildcard $(TESTDIR)*.c))))
KRUSKAL-TESTS = g-2-0 g-2-1 g-2-2 g-2-3 g-2-4 g-2-5 g-2-6 g-2-7 g-2-8 g-2-9 g-2-10

all : $(MAIN)

copy: all
	cp -f *.o $(TESTDIR)
	cp -f *.h $(TESTDIR)
	cp -f kruskal $(TESTDIR)

test-kruskal: all copy
	for x in $(KRUSKAL-TESTS); do \
			echo; echo -------; echo $$x.expected; echo -------; cat $(TESTDIR)$$x.expected; \
			./kruskal $(TESTDIR)$$x > $(TESTDIR)$$x.yours; \
			echo -------; echo $$x.yours; echo -------; cat $(TESTDIR)$$x.yours; echo -------; \
			cmp --silent $(TESTDIR)$$x.expected $(TESTDIR)$$x.yours && echo "PASSED" || echo "FAILED"; echo -------; \
	done

valgrind-kruskal: all copy
	for x in $(KRUSKAL-TESTS); do \
			valgrind --log-file=$(TESTDIR)$$x.valgrind kruskal $(TESTDIR)$$x; \
			echo; echo -------; echo $$x.valgrind; echo -------;  cat $(TESTDIR)$$x.valgrind; echo; \
	done

tester : all $(TESTS)
	for x in $(TESTS); do \
			echo; echo -------; echo $$x.expected; echo -------; cat $(TESTDIR)$$x.expected; \
			./$(TESTDIR)$$x > $(TESTDIR)$$x.yours; \
			echo -------; echo $$x.yours; echo -------; cat $(TESTDIR)$$x.yours; echo -------; \
			cmp --silent $(TESTDIR)$$x.expected $(TESTDIR)$$x.yours && echo "PASSED" || echo "FAILED"; echo -------; \
	done
	
valgrind : all $(TESTS)
	for x in $(TESTS); do \
			valgrind --log-file=$(TESTDIR)$$x.valgrind $(TESTDIR)$$x; \
			echo; echo -------; echo $$x.valgrind; echo -------;  cat $(TESTDIR)$$x.valgrind; echo; \
	done

$(TESTS): %: $(TESTDIR)%.c copy
	gcc $(LOPTS) -o $(TESTDIR)$@ $< $(OBJS)

kruskal : $(OBJS) kruskal.c cda.c
	gcc $(LOPTS) $(OBJS) kruskal.c -o kruskal

vertex.o : vertex.c vertex.h
	gcc $(OOPTS) vertex.c

edge.o : edge.c edge.h
	gcc $(OOPTS) edge.c

set.o : set.c set.h da.o
	gcc $(OOPTS) set.c

stack.o : stack.c stack.h
	gcc $(OOPTS) stack.c

integer.o: integer.c integer.h
	gcc $(OOPTS) integer.c

real.o: real.c real.h
	gcc $(OOPTS) real.c

queue.o : queue.c queue.h cda.o
	gcc $(OOPTS) queue.c

da.o : da.c da.h
	gcc $(OOPTS) da.c

cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c

tnode.o : tnode.c tnode.h
	gcc $(OOPTS) tnode.c

bst.o : bst.c bst.h tnode.o queue.o
	gcc $(OOPTS) bst.c
	
gst.o : gst.c gst.h bst.o
	gcc $(OOPTS) gst.c

rbt.o : rbt.c rbt.h gst.o
	gcc $(OOPTS) rbt.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

clean	:
		rm -f $(MAIN) *.o