# Generated automatically from Makefile.in by configure.
# Makefile of innbbsd for Firebird BBS 3.0
#
# $Id: Makefile.in,v 1.2 2004/06/29 14:59:22 rwzmm Exp $
##########################################################

VERSION		= 0.50beta-5F
NCMVERSION	= NoCeM_0.66

ADMINUSER = SYSOP.bbs@nykz.net
BBSHOME = /home/bbs/bbshome
BBSHOST = nykz.net
BBS_SRC = /home/bbs/bbssrc/fb2k
PERL	= /usr/bin/perl

LANG = GB
## There'are two other option of LANG: BIG5, GB and ENGLISH

# 这里是本人修改的地方，如果你有好的 Big5 <--> Gb 的处理工具，
# 则可以打开下面的 # 
#FILTER=-DFILTER -DBIG2GB -DGB2BIG -DHZ2GB
#FILTEROBJ=filter.o wildmat.o \
#	 convert/hztty/io.o convert/hztty/b2g.o \
#	 convert/hztty/b2g_tables.o \
#	 convert/hztty/hz2gb.o \
#	 convert/hztty/log.o \
# 这里结束了。

# if you want to do gb <--> big5 converstion on the fly
# You should install HC or WORDGB before adding this line
#FILTER=-DFILTER -DBIG2GB -DGB2BIG -DHZ2GB
#FILTER=-DFILTER -DBIG2GB -DGB2BIG -DHZ2GB -DBIG2JIS -DJIS2BIG
#FILTER=WORDG2B
#FILTEROBJ= filter.o wildmat.o io.o b2g.o b2g_tables.o hz2gb.o log.o

TARGET	= $(INNBBSD)
#TARGET	= $(BBSNNRP)
#TARGET	= $(BBSLINK)
#TARGET	= $(BBSNNRP) $(BBSLINK)

CC=gcc
INSTALL=cp
OPT= $(CFLAGS) -DUSE_NCM_PATCH

## -= you can add some defines above to active/deactive some function =-
##
## * add -DPGP5 if you want to use PGP
## Remove -DUSE_NCM_PATCH if you dont want to use NoCeM patch for your
## innbbsd system. (see README.nocem)
## * add -DANTISPAM if you want add "bbsnnrp patch" for your
## innbbsd system. (see README.antispam, *not suggested*)
## * add -DDONT_REGISTER if you dont want join NoCeM stats or slow connection
## to TANet. (see CHANGES.nocem)

BBS_DEP		= FirebirdBBS
BBS_REC		= $(BBS_SRC)/src/record.c r_port.o
BBS_POST	= innd/bbspost

mCFLAGS		= $(OPT) -I. -I$(BBS_SRC)/include -D$(BBS_DEP) $(EXTRAFLAGS) \
	-DDBZDEBUG -D_PATH_BBSHOME=\"$(BBSHOME)\" -DADMINUSER=\"$(ADMINUSER)\" \
	-DBBSHOST=\"$(BBSHOST)\" $(FILTER) -DLANG=\"$(LANG)\"

####################################################

OBJS = nocem.o inndchannel.o innbbsd.o connectsock.o rfc931.o \
       daemon.o file.o pmain.o parsdate.o his.o dbz.o \
       closeonexec.o dbztool.o inntobbs.o echobbslib.o receive_article.o \
       qp_conv.o lang.o \
       $(BBS_REC) $(FILTEROBJ)
SRCS = nocem.c inndchannel.c innbbsd.c connectsock.c rfc931.c \
       daemon.c file.c pmain.c parsdate.y his.c dbz.c \
       closeonexec.c dbztool.c inntobbs.c bbslib.c receive_article.c \
       qp_conv.c lang.c \
       port.c filter.c wildmat.c

MOBJS    = makedbz.o bbslib.o file.o dbz.o closeonexec.o 
HOBJS    = mkhistory.o bbslib.o file.o his.o dbz.o port.o closeonexec.o
DBZOBJS  = dbzserver.o dbzinnbbsd.o pmain.o connectsock.o bbslib.o his.o \
	   daemon.o file.o dbz.o dbztool.o rfc931.o port.o closeonexec.o
NNRPOBJS = bbsnnrp.o pmain.o bbslib.o connectsock.o file.o killspam.o
LINKOBJS = bbslink.o pmain.o inntobbs.o echobbslib.o connectsock.o file.o \
	   port.o  parsdate.o lang.o $(FILTEROBJ)
CTLOBJS  = ctlinnbbsd.o pmain.o bbslib.o connectsock.o file.o 
INNBBSD  = innbbsd makedbz mkhistory dbzserver bbsnnrp \
	   ctlinnbbsd $(BBS_POST) dbz_query_sample $(BBSLINK) 
BBSNNRP  = bbsnnrp
BBSLINK  = bbslink
EXES     = $(TARGET)

.c.o:
	$(CC) -c $(mCFLAGS) $*.c


all: linux

help:
	@echo "Your Configurations are listed as follows" 
	@echo "  CC = $(CC)"
	@echo "  BBSHOME = $(BBSHOME)"
	@echo "  BBS_SRC = $(BBS_SRC)"
	@echo "  BBS_DEP = $(BBS_DEP)"
	@echo " BBS_POST = $(BBS_POST)"
	@echo "ADMINUSER = $(ADMINUSER)"
	@echo "Change them in Makefile if not correct."
	@echo ""
	@echo "Please enter 'make sys-type', "
	@echo " make sun     : for Sun-OS 4.x and maybe some BSD systems, cc or gcc" 
	@echo " make linux   : for Linux 1.x"
	@echo " make solaris : for Sun-OS 5.x gcc" 
	@echo " make aix     : for IBM AIX" 
	@echo " make hpux    : for HP UX"
	@echo " make irix    : for IRIS"
	@echo " make alpha   : for DEC Alpha running OSF/1" 
	@echo " make freebsd : for BSD 4.4 systems" 
	@echo " make bsd     : for BSD systems, cc or gcc, if not in the above listts" 
	@echo " make sysv    : for SYSV systems, cc or gcc, if not in the above lists" 

sun: 
	@$(MAKE) EXTRAFLAGS="-DMMAP -DGETRUSAGE -DMALLOCMAP" LDFLAGS="$(DEBUGOBJ)" target

aix: 
	@$(MAKE) CC=cc EXTRAFLAGS="-DMMAP -DGETRUSAGE -DAIX -DSYSV" target

linux: 
	(cd convert/hztty; make linux; cd ../.. );
	@$(MAKE) EXTRAFLAGS="-DLINUX -DGETRUSAGE" target

hpux: 
	@$(MAKE) OPT= EXTRAFLAGS="-DMMAP -DHPUX -DSYSV" target

irix: 
	@$(MAKE) EXTRAFLAGS="-DMMAP -DIRIX -DSYSV" target

solaris:
	@$(MAKE) EXTRAFLAGS="-DMMAP -DSOLARIS -DSYSV" LDFLAGS="-lsocket -lnsl" YACC="@YACC@" target

freebsd:
	@$(MAKE) CC=cc EXTRAFLAGS="-DBSD44 -DMMAP -DGETRUSAGE" LIBCRYPT=-lcrypt  target

bsd:
	@$(MAKE) CC=cc EXTRAFLAGS="-DMMAP -DGETRUSAGE" target

alpha:
	@$(MAKE) CC=cc EXTRAFLAGS="-DMMAP -DDEC_ALPHA -DGETRUSAGE" target

sysv: 
	@$(MAKE) OPT= EXTRAFLAGS="-DMMAP -DSYSV" target

target: $(EXES) bbsname

bbsname:
	rm -fr bbsname.bbs hisconvert
	echo 'NYKZBBS' > bbsname.bbs
	sed -e "s:@PERL@:$(PERL):" hisconvert.in > hisconvert

r_port.o: r_port.c
	$(CC) -c $(mCFLAGS) -DWITH_RECORD_O r_port.c 

echobbslib.o: echobbslib.c
	$(CC) -c $(mCFLAGS) -DWITH_ECHOMAIL echobbslib.c

$(BBS_POST): $(BBS_POST).c
	(cd $(INND_ROOT)innd; $(MAKE) CC=$(CC) CFLAGS="$(mCFLAGS)" BBS_SRC=$(BBS_SRC) PERL=$(PERL))

innbbspost.o: innbbspost.c
	$(CC) -c $(mCFLAGS) -I$(BBS_SRC) -c innbbspost.c 

makedbz: $(MOBJS)
	$(CC) $(OPT) -o makedbz $(MOBJS) $(LDFLAGS)

dbzserver.o: dbzserver.c
	$(CC) -c $(mCFLAGS) -DDBZSERVER -I$(BBS_SRC) -c dbzserver.c   

dbzinnbbsd.o: dbzinnbbsd.c
	$(CC) -c $(mCFLAGS) -DDBZSERVER -I$(BBS_SRC) -c dbzinnbbsd.c   

dbzserver: $(DBZOBJS)
	$(CC) $(OPT) -o dbzserver $(DBZOBJS) $(LDFLAGS)

bbsnnrp: $(NNRPOBJS)
	$(CC) -o bbsnnrp $(NNRPOBJS) $(OPT) $(LDFLAGS)

bbslink: $(LINKOBJS)
	$(CC) -o bbslink $(LINKOBJS) $(OPT) $(LDFLAGS)

ctlinnbbsd: $(CTLOBJS)
	$(CC) $(OPT) -o ctlinnbbsd $(CTLOBJS) $(LDFLAGS)

mkhistory: $(HOBJS)
	$(CC) $(OPT) -o mkhistory $(HOBJS) $(LDFLAGS)

dbz_query_sample: dbz_query_sample.o
	$(CC) $(OPT) -o dbz_query_sample dbz_query_sample.o $(LDFLAGS)


innbbsd: $(OBJS)
	$(CC) -o innbbsd $(OBJS) $(OPT) -I$(BBS_SRC)/include $(LDFLAGS)

install:
	@echo "Read README and do the following manually"
	@echo "copy innbbsd bbsnnrp mkhistory to $(BBSHOME)/innd"	
	@echo "copy innd/bbspost innd/bntplink to $(BBSHOME)/innd"	
	test -d $(BBSHOME)/innd || mkdir $(BBSHOME)/innd
	@echo "$(BBSHOME)/innd must be owner by 'bbs'"
	-mv -f $(BBSHOME)/innd/innbbsd $(BBSHOME)/innd/innbbsd.OLD
	$(INSTALL) innbbsd $(BBSHOME)/innd/innbbsd
	-mv -f $(BBSHOME)/innd/bbsnnrp $(BBSHOME)/innd/bbsnnrp.OLD
	$(INSTALL) bbsnnrp $(BBSHOME)/innd/bbsnnrp
	-mv -f $(BBSHOME)/innd/ctlinnbbsd $(BBSHOME)/innd/ctlinnbbsd.OLD
	$(INSTALL) ctlinnbbsd $(BBSHOME)/innd/ctlinnbbsd
	-mv -f $(BBSHOME)/innd/bbspost $(BBSHOME)/innd/bbspost.OLD
	$(INSTALL) $(BBS_POST) $(BBSHOME)/innd/bbspost
	-mv -f $(BBSHOME)/innd/bntplink $(BBSHOME)/innd/bntplink.OLD
	$(INSTALL) innd/bntplink $(BBSHOME)/innd/bntplink
	-mv -f $(BBSHOME)/innd/mkhistory $(BBSHOME)/innd/mkhistory.OLD
	$(INSTALL) mkhistory $(BBSHOME)/innd/mkhistory
	-mv -f $(BBSHOME)/innd/parsedate $(BBSHOME)/innd/parsedate.OLD 
	$(INSTALL) innd/parsedate $(BBSHOME)/innd/parsedate
	-mv -f $(BBSHOME)/innd/tcp.pl $(BBSHOME)/innd/tcp.pl.OLD 
	$(INSTALL) innd/tcp.pl $(BBSHOME)/innd/tcp.pl
	-mv -f $(BBSHOME)/innd/innbbslib.pl $(BBSHOME)/innd/innbbslib.pl.OLD 
	$(INSTALL) innd/innbbslib.pl $(BBSHOME)/innd/innbbslib.pl
	-mv -f $(BBSHOME)/innd/bbslink $(BBSHOME)/innd/bbslink.OLD 
	$(INSTALL) bbslink $(BBSHOME)/innd/bbslink
	-mv -f $(BBSHOME)/innd/mailpost $(BBSHOME)/innd/mailpost.OLD
	$(INSTALL) innd/mailpost $(BBSHOME)/innd/mailpost
	-mv -f $(BBSHOME)/innd/bmpost $(BBSHOME)/innd/bmpost.OLD
	$(INSTALL) innd/bmpost $(BBSHOME)/innd/bmpost
	-mv -f $(BBSHOME)/innd/inntobbs $(BBSHOME)/innd/inntobbs.OLD
	$(INSTALL) innd/inntobbs $(BBSHOME)/innd/inntobbs
	-mv -f $(BBSHOME)/innd/rfc931.pl $(BBSHOME)/innd/rfc931.pl.OLD
	$(INSTALL) innd/rfc931.pl $(BBSHOME)/innd/rfc931.pl
	-rm -f $(BBSHOME)/innd/bbsname.bbs
	$(INSTALL) bbsname.bbs $(BBSHOME)/innd/bbsname.bbs
	cp ncmperm.bbs $(BBSHOME)/innd/ncmperm.bbs

clean:
	rm -f *.o *~ $(EXES) core innd/src/*.o
	cd convert/hztty; make clean
	rm -fr bbsname.bbs hisconvert

cleanall: clean
	( cd innd; make clean ; cd .. )

depend:
	 makedepend -- $(mCFLAGS) -- $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

