FLAGS = -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE
GCOV_FLAG= -lcheck
GCOV_LIBS=--coverage
BUILD_PATH=./
SOURCES=s21_*.c helpers.c
H=s21_decimal.h
LIBO=*.o
LIBA=s21_decimal.a
EXE=test.out
TST_LIBS=-lcheck -lsubunit -lm -lpthread

ifeq ($(shell uname),Linux)
		GCOV_FLAG += -lsubunit -lrt -lm -lpthread
endif


all: s21_decimal.a

s21_decimal.a: clean 
	gcc ${FLAGS} -c $(SOURCES) -lm
	ar rcs $(LIBA) $(LIBO)
	ranlib $(LIBA) 

test: s21_decimal.a
	gcc  ${FLAGS} test_decimal.c -L. $(LIBA) $(TST_LIBS) -o $(BUILD_PATH)$(EXE)
	$(BUILD_PATH)$(EXE)

gcov_report: s21_decimal.a
	gcc $(FLAGS) $(GCOV_LIBS) test_decimal.c $(SOURCES) $(LIBA) -L. $(LIBA) $(TST_LIBS) -o $(BUILD_PATH)$(EXE)
	$(BUILD_PATH)$(EXE)
	lcov -t "test" -c -d $(BUILD_PATH) --output-file $(BUILD_PATH)coverage.info
	genhtml $(BUILD_PATH)coverage.info --output-directory $(BUILD_PATH)report/

rebuild: clean all

clean:
	rm -rf *.a && rm -rf *.o
	rm -rf *.info && rm -rf *.gcda && rm -rf *.gcno &&  rm -rf *.gcov
	rm -rf report/ && rm -rf *.out

check:
	@echo
	@echo ===STYLE TEST===
	@echo
	clang-format -style=Google -n *.c *.h
	@echo
	@echo ===VALGRIND TEST===
	@echo
	valgrind --tool=memcheck --leak-check=yes $(BUILD_PATH)$(EXE)	


