all: exe

exe: ./src/main.c ./src/hexDump.c
	 gcc ./src/main.c ./src/hexDump.c mpack/mpack-common.c mpack/mpack-expect.c mpack/mpack-node.c mpack/mpack-platform.c mpack/mpack-reader.c mpack/mpack-writer.c -o mpack-run -I "mpack/"

clean:
	 -rm mpack-run 2>/dev/null
