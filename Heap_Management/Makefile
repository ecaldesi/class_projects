CC=             gcc
CFLAGS=         -g -gdwarf-2 -std=gnu99 -Wall
LDFLAGS=
LIBRARIES=	lib/libmalloc-ff.so \
                lib/libmalloc-bf.so \
                lib/libmalloc-wf.so \
                lib/libmalloc-nf.so

EC_LIBRARIES=	lib/EC_libmalloc-ff.so \
                lib/EC_libmalloc-bf.so \
                lib/EC_libmalloc-wf.so \
                lib/EC_libmalloc-nf.so

all:    $(LIBRARIES)

lib/libmalloc-ff.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $< $(LDFLAGS)

# TODO: rules for remaining libraries
lib/libmalloc-bf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $< $(LDFLAGS)

lib/libmalloc-wf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $< $(LDFLAGS)

lib/libmalloc-nf.so:     src/malloc.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=3 -o $@ $< $(LDFLAGS)

extraCredit:	$(EC_LIBRARIES)

lib/EC_libmalloc-ff.so:     src/malloc_EC.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $< $(LDFLAGS)

# TODO: rules for remaining libraries
lib/EC_libmalloc-bf.so:     src/malloc_EC.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $< $(LDFLAGS)

lib/EC_libmalloc-wf.so:     src/malloc_EC.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $< $(LDFLAGS)

lib/EC_libmalloc-nf.so:     src/malloc_EC.c
	$(CC) -shared -fPIC $(CFLAGS) -DFIT=3 -o $@ $< $(LDFLAGS)

clean:
	rm -f $(LIBRARIES)
	rm -f $(EC_LIBRARIES)

.PHONY: all clean

