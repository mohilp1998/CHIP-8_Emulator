######### This make file is no longer in use. #########
######### Shifted to vscode for building and stuff #########
# CC = g++
# CFLAGS = -std=c++11 -Wall -g
# ALL_DEPENDENCY = main.o rom.o memory.o opcodeHandler.o

# all: $(ALL_DEPENDENCY)
# 	$(CC) $(CFLAGS) -o main $(ALL_DEPENDENCY)
# 	rm -f $(ALL_DEPENDENCY)
# main.o: main.cpp
# 	$(CC) $(CFLAGS) -c main.cpp
# rom.o: rom.cpp
# 	$(CC) $(CFLAGS) -Wno-sign-compare -c rom.cpp
# memory.o: memory.cpp
# 	$(CC) $(CFLAGS) -c memory.cpp
# opcodeHandler.o:
# 	$(CC) $(CFLAGS) -c opcodeHandler.cpp