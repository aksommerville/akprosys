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

static int _akps_param(void *userdata,const char *k,int kc,const char *v,int vc) {
  return 0;
}
  
static int _akps_init(void *userdata) {
  return 0;
}

static void _akps_quit(void *userdata) {
  prosystem_Close();
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
  
static int _akps_update(void *userdata) {
  prosystem_ExecuteFrame(input_state);
  eh_hi_color_table(palette_data);//TODO This shouldn't be necessary every frame, should it?
  
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
  //uint8_t lo=*pcmsrc,hi=*pcmsrc;
  for (;i-->0;pcmsrc++,pcmdst++) {
    *pcmdst=(*pcmsrc)<<7;
    //if (*pcmsrc<lo) lo=*pcmsrc; else if (*pcmsrc>hi) hi=*pcmsrc;
  }
  //fprintf(stderr,"%02x..%02x\n",lo,hi);
  
  return eh_hi_frame(maria_surface,pcm,pcmsrcc);
}
  
static int _akps_player_input(void *userdata,int plrid,int btnid,int value,int state) {
  switch (plrid) {
    case 1: switch (btnid) {
        case EH_BUTTON_LEFT: input_state[1]=value; break;
        case EH_BUTTON_RIGHT: input_state[0]=value; break;
        case EH_BUTTON_UP: input_state[3]=value; break;
        case EH_BUTTON_DOWN: input_state[2]=value; break;
        case EH_BUTTON_A: input_state[4]=value; break;
        case EH_BUTTON_B: input_state[5]=value; break;
        case EH_BUTTON_C: input_state[14]=value; break; // Pause
        case EH_BUTTON_AUX1: input_state[12]=value; break; // Reset
        case EH_BUTTON_AUX2: input_state[13]=value; break; // Select
        case EH_BUTTON_L: input_state[15]=value; break; // Left Difficulty
        case EH_BUTTON_R: input_state[16]=value; break; // Right Difficulty
      } break;
    case 2: switch (btnid) {
        case EH_BUTTON_LEFT: input_state[7]=value; break;
        case EH_BUTTON_RIGHT: input_state[6]=value; break;
        case EH_BUTTON_UP: input_state[9]=value; break;
        case EH_BUTTON_DOWN: input_state[8]=value; break;
        case EH_BUTTON_A: input_state[10]=value; break;
        case EH_BUTTON_B: input_state[11]=value; break;
        case EH_BUTTON_C: input_state[14]=value; break; // Pause
        case EH_BUTTON_AUX1: input_state[12]=value; break; // Reset
        case EH_BUTTON_AUX2: input_state[13]=value; break; // Select
        case EH_BUTTON_L: input_state[15]=value; break; // Left Difficulty
        case EH_BUTTON_R: input_state[16]=value; break; // Right Difficulty
      } break;
  }
  return 0;
}
  
static int _akps_text_input(void *userdata,int codepoint) {
  return 0;
}
  
static int _akps_reset(void *userdata,const char *path) {
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
  
  struct eh_hi_delegate delegate={
    .video_rate=prosystem_frequency,
    .video_width=320,
    .video_height=prosystem_scanlines,
    .video_format=EH_VIDEO_FORMAT_I8,
    .audio_rate=31440,
    .audio_chanc=1,
    .audio_format=EH_AUDIO_FORMAT_S16,
    .playerc=2,
    .appname="akprosys",
    .userdata=0,
    .param=_akps_param,
    .init=_akps_init,
    .quit=_akps_quit,
    .update=_akps_update,
    .player_input=_akps_player_input,
    .text_input=_akps_text_input,
    .reset=_akps_reset,
  };
  return eh_hi_main(&delegate,argc,argv);
}
