all:
.SILENT:
.SECONDARY:
PRECMD=echo "  $(@F)" ; mkdir -p $(@D) ;

SRCDIRS:=Core Emuhost
MIDDIR:=mid
OUTDIR:=out

CC:=gcc -c -MMD -O3 $(addprefix -I,$(SRCDIRS)) -I../ra2/src/emuhost
CXX:=g++ -c -MMD -O3 $(addprefix -I,$(SRCDIRS)) -I../ra2/src/emuhost
LD:=g++
LDPOST:=../ra2/out/linux-default/libemuhost.a -lz -lX11 -lGL -ldrm -lgbm -lEGL -lpulse-simple -lasound

SRCFILES:=$(shell find $(SRCDIRS) -type f)
CFILES:=$(filter %.c %.cpp,$(SRCFILES))
OFILES:=$(patsubst %,$(MIDDIR)/%.o,$(basename $(CFILES)))
-include $(OFILES:.o=.d)

$(MIDDIR)/%.o:%.c;$(PRECMD) $(CC) -o$@ $<
$(MIDDIR)/%.o:%.cpp;$(PRECMD) $(CXX) -o$@ $<

EXE:=$(OUTDIR)/akprosys
all:$(EXE)
$(EXE):$(OFILES);$(PRECMD) $(LD) -o$@ $^ $(LDPOST)

clean:;rm -rf $(MIDDIR) $(OUTDIR)

#run:$(EXE);$(EXE) ~/rom/atari7800/DARKCHAM.A78
#run:$(EXE);$(EXE) ~/rom/atari7800/CENTIPED.A78
#run:$(EXE);$(EXE) ~/rom/atari7800/DKONG.A78
#run:$(EXE);$(EXE) ~/rom/atari7800/ballblaz.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/xenophobe.a78
#run:$(EXE);$(EXE) ~/rom/atari7800/ikariwar.a78
run:$(EXE);$(EXE) ~/rom/atari7800/rescue.a78
