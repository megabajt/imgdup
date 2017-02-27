NULL =

AM_CPPFLAGS =				\
	$(GLIB_CFLAGS)			\
	$(PHASH_CFLAGS)			\
	$(NULL)

bin_PROGRAMS = imgdup

imgdup_SOURCES =			\
	main.c				\
	engine.c			\
	result.c			\
	phash.cpp			\
	$(NULL)

imgdup_LDADD = 				\
	$(GLIB_LIBS)			\
	$(PHASH_LIBS)			\
	$(NULL)

-include $(top_srcdir)/git.mk
