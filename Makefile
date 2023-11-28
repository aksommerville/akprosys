all:
.SILENT:
.SECONDARY:
PRECMD=echo "  $(@F)" ; mkdir -p $(@D) ;

SRCDIRS:=Core Emuhost
MIDDIR:=mid
OUTDIR:=out

LIBEMUHOST:=../ra3/out/libemuhost.a 

CC:=gcc -c -MMD -O3 $(addprefix -I,$(SRCDIRS)) -I../ra3/out/include
CXX:=g++ -c -MMD -O3 $(addprefix -I,$(SRCDIRS)) -I../ra3/out/include
LD:=g++
LDPOST:=$(LIBEMUHOST) -lz -lX11 -lXinerama -lGL -ldrm -lgbm -lEGL -lpulse-simple -lasound

SRCFILES:=$(shell find $(SRCDIRS) -type f)
CFILES:=$(filter %.c %.cpp,$(SRCFILES))
OFILES:=$(patsubst %,$(MIDDIR)/%.o,$(basename $(CFILES)))
-include $(OFILES:.o=.d)

$(MIDDIR)/%.o:%.c;$(PRECMD) $(CC) -o$@ $<
$(MIDDIR)/%.o:%.cpp;$(PRECMD) $(CXX) -o$@ $<

EXE:=$(OUTDIR)/akprosys
all:$(EXE)
$(EXE):$(OFILES) $(LIBEMUHOST);$(PRECMD) $(LD) -o$@ $^ $(LDPOST)

clean:;rm -rf $(MIDDIR) $(OUTDIR)

# ok:
run:$(EXE);$(EXE) ~/rom/atari7800/dark-chambers.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/centipede.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/donkey-kong.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/ball-blazer.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/xenophobe.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/ikari-warriors.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/pitfighter.a78

# black screen:
#run:$(EXE);$(EXE) ~/rom/atari7800/F14.bin # CARTRIDGE_TYPE_NORMAL
#run:$(EXE);$(EXE) ~/rom/atari7800/double-dragon.a78 # CARTRIDGE_TYPE_SUPERCART
#run:$(EXE);$(EXE) ~/rom/atari7800/rampage.a78 # CARTRIDGE_TYPE_SUPERCART

# other issues:
#run:$(EXE);$(EXE) ~/rom/atari7800/F18.A78
#run:$(EXE);$(EXE) ~/rom/atari7800/rescue-on-fractalus.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/kung-fu-master.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/mia.a78
