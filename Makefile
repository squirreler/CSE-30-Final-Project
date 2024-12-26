# UCM Makefile for C++ projects
# Angelo Kyrilov

WORKDIR = $(shell pwd)

NOT_ALLOWED = 'stl_vector|stl_list|stl_deque|std_stack|stl_queue|stl_set|stl_map|unordered_map|unordered_set|algorithmfwd.h'
DEP = $(shell g++ -M main.cpp | grep -E $(NOT_ALLOWED))


build_config = '{\n\
"version": "0.2.0", \n\
"configurations": [\n\
{\n\
"type": "lldb",\n\
"request": "launch",\n\
"name": "Debug Current",\n\
"program": "$(WORKDIR)/$(1)",\n\
"args": [],\n \
"cwd": "$(WORKDIR)"\n\
}\n\
]\n\
}'

CONFIG_FOLDER = ~/workspace/.theia/
CONFIG_FILE = ~/workspace/.theia/launch.json

SKIPTIME=0

all:
	@rm -rf app
	g++ main.cpp -g -I. -o app

run:
	@rm -rf app
	g++ main.cpp -g -I. -o app
	./app
