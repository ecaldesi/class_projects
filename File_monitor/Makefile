CC=		g++
CFLAGS=		-Wall --std=c++11
LD=		g++
LDFLAGS=	-L
AR=		ar
ARFLAGS=	rcs
TARGETS=	rorschach \
		fs_monitor.o \

all:		$(TARGETS)

rorschach:		rorschach.cpp fs_monitor.o
		@echo Compiling rorschach...
		@ $(CC) $(CFLAGS) -o rorschach rorschach.cpp fs_monitor.o

fs_monitor.o:	fs_monitor.cpp
		@echo Compiling fs_monitor.o...
		@$(CC) $(CFLAGS) -c -o fs_monitor.o fs_monitor.cpp

clean:
		@echo Cleaning...
		@rm -f $(TARGETS) *.o *.log *.input

.PHONY:		all test benchmark clean

