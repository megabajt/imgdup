NULL =

AM_CPPFLAGS =				\
	$(GLIB_CFLAGS)			\
	$(GDK_PIXBUF_CFLAGS)		\
	$(PHASH_CFLAGS)			\
	$(NULL)

bin_PROGRAMS = imgdup

imgdup_SOURCES =			\
	main.c				\
	engine.c			\
	result.c			\
	hash.cpp			\
	$(NULL)

imgdup_LDADD = 				\
	$(GLIB_LIBS)			\
	$(GDK_PIXBUF_LIBS)		\
	$(PHASH_LIBS)			\
	$(NULL)

-include $(top_srcdir)/git.mk
