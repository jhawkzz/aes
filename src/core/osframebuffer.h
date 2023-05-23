
// See License.md for license

#ifndef OSFRAMEBUFFER_H_
#define OSFRAMEBUFFER_H_

// On MVSX, the display driver is reporting a framerate of 8hz.
// Reading from “/sys/devices/platform/act_fb.0/graphics/fb0/modes” shows 
// "U:1280x1024-8" (1280x1024 @ 8hz)
// The timings reported from FBIOGET_VSCREENINFO confirm it. They are:
// mode "1280x1024-8"
//	 # D: 12.700 MHz, H: 8.581 kHz, V: 8.141 Hz
//	 geometry 1280 1024 1280 2048 16
//	 timings 78740 60 80 10 10 60 10
//	 accel false
//	 rgba 5/11,6/5,5/0,0/0
//endmode
//
// However, my eyes and testing tell me its closer to 20-30, and 60 if you're ok with flickering.
// (FBA has flickering if I clear the back buffer)
// Reading "/sys/devices/platform/panel-glp.0/owl_panel/lcd/default_mode" shows
// "1280x1024p-60Hz" which seems to confirm the monitor is certainly capable of more.
//
// Looking at /misc/bin_cfg.xml shows they are literally setting the driver timings to the above 8hz:
// <item name = "dclk" type = "int" value = "127000"/>
//	<item name = "data_width" type = "int" value = "24"/>
//	<item name = "resolution" type = "int" value = "1280;1024;60"/>
//	<item name = "draw_size" type = "int" value = "1280;1024"/>
//	<item name = "is_primary" type = "int" value = "1"/>
//	<item name = "physical_area" type = "int" value = "296;285"/>
//	<item name = "pixel_clock" type = "int" value = "78740"/>
//      <item name = "hsync_sp" type = "int" value = "60"/>
//      <item name = "hsync_fp" type = "int" value = "60"/>
//      <item name = "hsync_bp" type = "int" value = "80"/>
//      <item name = "vsync_sp" type = "int" value = "10"/>
//      <item name = "vsync_fp" type = "int" value = "10"/>
//
// I suspect the drivers are a fork from the Neo Geo Mini, which probably did
// run at 8hz for that tiny lcd screen, and internally its doing something
// with the reported timings to scale them down and be faster. The fact that
// the fb mode is "U:1280x1024-8" means "Unknown Standard" which implies they could
// be doing anything they want internally with that 8hz.
// Additionally seeing the explicitely set timings further implies that.
// Lastly, mVScreenInfo.activate = FB_ACTIVATE_VBL; and ioctl(mFrameBufferHandle, FBIO_WAITFORVSYNC, 0 );
// both report as supported but dont seem to do anything. More evidence they're not adhearing
// to standard stuff.        
// 
// The only other thing I could possibly check would be to see if the stock emu (mvsnjemu)
// is changing the display mode when it boots.
//
// The takeaway is this:
// If you dont care about flickering, render away, dont clear the backbuffer,
// and the tearing will be minimal.
// If you do not want flickering, throttle the rendering to no more than about 20fps
// or 48ms.
#define FB_NOFLICKER_RENDER_TIME_MS (48)

// the amount of bytes from the base frame buffer for the start of the double buffer
#define FB_DOUBLE_BUFFER_OFFSET_BYTES (LCD_HEIGHT * SCREEN_PITCH)

//The path to the frame buffer out on the MVSX Linux OS
#define FB_DEVICE "/dev/graphics/fb0"

class OSFrameBuffer
{
public:
		static int    Create( );
		static void   Destroy( );
		
		static short *GetBackBuffer( );
        static void   Flip( );
        
        static void   ToggleRenderThrottle( int enabled );
        static int    RenderThrottleEnabled( );
        
        static void   ToggleClearBackBuffer( int enabled );
        static int    ClearBackBufferEnabled( );
private:
        static void   PrintMode( fb_var_screeninfo *pfbVScreenInfo );
        static void   PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo );
        static void   PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo );
        
        static int    mThrottleRendering; //if true, use the time below. otherwise just render.
        static int    mFBNoFlickerRenderTimeMS;
        static int    mClearBackBuffer; //set to false to totally eliminate flicker
        
        static int    mFrameBufferHandle;
        static int    mFrameBufferBytes;
        
        static char  *mpFrameBuffer;
        static char  *mpBackBuffer;
        static fb_var_screeninfo mVScreenInfo;
};

#endif
