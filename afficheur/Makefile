JM = java
JMFLAGS = -cp build -ea
JC = javac
JCFLAGS = -cp build -d build
IP ?= localhost
PORT ?= 8000

all : build 

# compilation
build: 
	rm -rf build
	mkdir build
	${JC} ${JCFLAGS} Client.java 

# clean compiled files
clean: 
	rm -rf build 

client: 
	${JM} ${JMFLAGS} Client $(IP) $(PORT)

.PHONY : build client clean 
