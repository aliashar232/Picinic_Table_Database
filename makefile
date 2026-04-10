CC = gcc
CFLAGS = -Wall -std=c99

all: dashboard

dashboard: dashboard.o DB.o DB_impl.o 
	$(CC) $(CFLAGS) -o dashboard dashboard.o DB.o DB_impl.o

dashboard.o: dashboard.c
	$(CC) $(CFLAGS) -c dashboard.c

DB.o: DB.c
	$(CC) $(CFLAGS) -c DB.c

DB_impl.o: DB_impl.c
	$(CC) $(CFLAGS) -c DB_impl.c

test: run_tests

run_tests: test.o DB.o DB_impl.o 
	$(CC) $(CFLAGS) -o run_tests test.o DB.o DB_impl.o

test.o: testing_strategy/tests.c
	$(CC) $(CFLAGS) -c testing_strategy/tests.c -o test.o

testing: run_tests
	./run_tests

	diff testing_strategy/test_out.csv testing_strategy/expected_exportDB_output.csv
	echo "Export DB test passed!"
	

	diff testing_strategy/sorted_output.csv testing_strategy/expected_sorted_output.csv
	echo "Sort by name test passed!"
	

	diff testing_strategy/editTableEntry_output.csv testing_strategy/expected_editTableEntry_output.csv
	echo "Edit Table Entry test passed!"
	

	diff testing_strategy/reportByWard_output.csv testing_strategy/expected_reportByWard_output.csv
	echo "Report by Ward test passed!"

	diff testing_strategy/reportByNeighbourhood_output.csv testing_strategy/expected_reportByNeighbourhood_output.csv
	echo "Report by Neighbourhood test passed!"


clean:
	rm -f *.o dashboard tests
	rm -f testing_strategy/*.o run_tests