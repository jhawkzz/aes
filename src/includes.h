
// See License.md for license

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <string>
#include <iostream>
#include <linux/input.h>
#include <poll.h>
#include <sys/mount.h>
#include <fstream>
#include <sys/reboot.h>
#include <dirent.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "defines.h"
#include "util/util.h"
#include "util/ringbuffer.h"
#include "main.h"
#include "core/ostimer.h"

#ifdef MVSX
#include "core/mvsxinput.h"
#elif ASP
#include "core/aspinput.h"
#endif

#include "core/osmount.h"
#include "core/osaudio.h"
#include "core/osframebuffer.h"
#include "core/osprocess.h"
#include "core/osled.h"
#include "audio/waveloader.h"
#include "audio/audiomanager.h"
#include "render/bitmaploader.h"
#include "render/font.h"
#include "render/drawlistobject.h"
#include "render/renderer.h"
#include "render/menuitem.h"
#include "game/state/basegamestate.h"
#include "game/state/statecheckleds.h"
#include "game/state/statemainmenu.h"
#include "game/state/statecheckinputs.h"
#include "game/state/stateexec.h"
#include "game/state/statemounting.h"
#include "game/state/statecheckaudio.h"
#include "game/state/statequit.h"
#include "game/state/statecredits.h"
#include "game/gameinput.h"
#include "game/gameconfig.h"
#include "game/game.h"

#endif
