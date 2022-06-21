all: exe

exe: ./src/main.c ./src/hexDump.c
	 gcc ./src/main.c ./src/hexDump.c -o mpack-run

clean:
	 -rm mpack-run 2>/dev/null
