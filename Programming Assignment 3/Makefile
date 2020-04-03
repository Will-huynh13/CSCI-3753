PA3:multi-lookup.c util.c
	gcc -Wall -Wextra multi-lookup.c -o multi-lookup util.c -pthread -g -w

clean:
	rm *.txt multi-lookup
	clear

test1:
	./multi-lookup 1 1 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt
test2:
	./multi-lookup 1 3 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt
test3:
	./multi-lookup 3 1 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt


test4:
	./multi-lookup 3 3 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt

test5:
	./multi-lookup 5 5 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt

test6:
	./multi-lookup 8 5 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt

val:
	valgrind --leak-check=full --show-leak-kinds=all ./multi-lookup 2 2 results.txt services.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt
