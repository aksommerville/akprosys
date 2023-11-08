#include <stdio.h>
#include "ProSystem.h"
#include "Cartridge.h"
#include "Maria.h"
#include "Palette.h"
#include "Pokey.h"
#include "Tia.h"

extern "C" {
#include "emuhost.h"
}

static int16_t pcm[TIA_BUFFER_SIZE]={0};

// See Riot.cpp:
// | 00       | Joystick 1   | Right
// | 01       | Joystick 1   | Left
// | 02       | Joystick 1   | Down
// | 03       | Joystick 1   | Up
// | 04       | Joystick 1   | Button 1
// | 05       | Joystick 1   | Button 2
// | 06       | Joystick 2   | Right
// | 07       | Joystick 2   | Left
// | 08       | Joystick 2   | Down
// | 09       | Joystick 2   | Up
// | 10       | Joystick 2   | Button 1
// | 11       | Joystick 2   | Button 2
// | 12       | Console      | Reset
// | 13       | Console      | Select
// | 14       | Console      | Pause
// | 15       | Console      | Left Difficulty
// | 16       | Console      | Right Difficulty
static uint8_t input_state[17]={0};
static uint8_t ldiff=0,rdiff=0;
static uint16_t p1pv=0,p2pv=0;

static void akps_update_input_player(int p,uint16_t srcstate) {
  input_state[p+0]=(srcstate&EH_BTN_RIGHT)?1:0;
  input_state[p+1]=(srcstate&EH_BTN_LEFT)?1:0;
  input_state[p+2]=(srcstate&EH_BTN_DOWN)?1:0;
  input_state[p+3]=(srcstate&EH_BTN_UP)?1:0;
  input_state[p+4]=(srcstate&EH_BTN_SOUTH)?1:0;
  input_state[p+5]=(srcstate&EH_BTN_WEST)?1:0;
}

static void akps_update_input() {
  uint16_t p1=eh_input_get(1);
  uint16_t p2=eh_input_get(2);
  if (p1!=p1pv) {
    if ((p1&EH_BTN_AUX1)&&!(p1pv&EH_BTN_AUX1)) ldiff=ldiff?0:1;
    p1pv=p1;
  }
  if (p2!=p2pv) {
    if ((p2&EH_BTN_AUX1)&&!(p2pv&EH_BTN_AUX1)) rdiff=rdiff?0:1;
    p2pv=p2;
  }
  akps_update_input_player(0,p1);
  akps_update_input_player(6,p2);
  input_state[12]=p1&EH_BTN_R1; // Reset
  input_state[13]=p1&EH_BTN_AUX2; // Select
  input_state[14]=p1&EH_BTN_L1; // Pause
  input_state[15]=ldiff;
  input_state[16]=rdiff;
}
  
static int _akps_update(int partial) {

  akps_update_input();

  prosystem_ExecuteFrame(input_state);
  eh_ctab_write(0,256,palette_data);//TODO This shouldn't be necessary every frame, should it?
  
  const uint8_t *pcmsrc;
  int pcmsrcc;
  if (cartridge_pokey) {
    pcmsrc=pokey_buffer;
    pcmsrcc=pokey_size;
  } else {
    pcmsrc=tia_buffer;
    pcmsrcc=tia_size;
  }
  int16_t *pcmdst=pcm;
  int i=pcmsrcc;
  for (;i-->0;pcmsrc++,pcmdst++) {
    *pcmdst=(*pcmsrc)<<7;
  }
  
  eh_video_write(maria_surface);
  eh_audio_write(pcm,pcmsrcc);
  
  return 0;
}

static int _akps_load_file(const char *path) {
  if (path) {
    if (cartridge_Load(path)) {
      fprintf(stderr,"%s: Loaded\n",path);
    } else {
      fprintf(stderr,"%s: cartridge_Load failed\n",path);
      return -1;
    }
    prosystem_Reset();
  } else {
    prosystem_Reset();//TODO does this also unload? that would be a problem
  }
  return 0;
}

/* Main.
 */

int main(int argc,char **argv) {
  
  prosystem_Reset();
  /*
  #define LOGVAR(k) fprintf(stderr,"%s: %d\n",#k,k);
  LOGVAR(prosystem_active)
  LOGVAR(prosystem_paused)
  LOGVAR(prosystem_frequency)
  LOGVAR(prosystem_frame)
  LOGVAR(prosystem_scanlines)
  LOGVAR(prosystem_cycles)
  #undef LOGVAR
  /**/
  
  struct eh_delegate delegate={
    .name="akprosys",
    .iconrgba=0,//TODO
    .iconw=0,
    .iconh=0,
    .video_width=320,
    .video_height=prosystem_scanlines,
    .video_format=EH_VIDEO_FORMAT_I8,
    .video_rate=(double)prosystem_frequency,
    .audio_rate=31440,
    .audio_chanc=1,
    .audio_format=EH_AUDIO_FORMAT_S16N,
    .playerc=2,
    .load_file=_akps_load_file,
    .update=_akps_update,
  };
  int status=eh_main(argc,argv,&delegate);
  if (!status) prosystem_Close();
  return status;
}
