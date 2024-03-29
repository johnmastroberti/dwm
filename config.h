/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono-Regular:size=16",
                                        "DejaVuSansMono Nerd Font:size=16",
                                        "JoyPixels:pixelsize=10:antialias=true:autohint=true"};
static char dmenufont[]             = "JetBrainsMono-Regular:size=16";
//static char normbgcolor[]           = "#171c28";
//static char normbordercolor[]       = "#444444";
//static char normfgcolor[]           = "#5ccfe6";
//static char selfgcolor[]            = "#171c28";
//static char selbordercolor[]        = "#5ccfe6";
//static char selbgcolor[]            = "#5ccfe6";
static char normfgcolor[]           = "#f7f7f7";
static char normbordercolor[]       = "#2a2926";
static char normbgcolor[]           = "#2a2926";
static char selbgcolor[]            = "#9c5e21";
static char selbordercolor[]        = "#9c5e21";
static char selfgcolor[]            = "#f7f7f7";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor }
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "python", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
};

/* tagging */
static const char *tags[] = { "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Evolution",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ "zoom",     NULL,       NULL,       	    0,            1,           0,         0,        -1 },
	{ "trayer",   NULL,       NULL,       	    (1 << 9)-1,   0,           0,         0,        -1 },
	{ "St",       NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ "popup",    NULL,       NULL,       	    0,            1,           0,         1,        -1 },
	{ NULL,       "popup",    NULL,       	    0,            1,           0,         1,        -1 },
	{ NULL,       NULL,       "popup",       	  0,            1,           0,         1,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ "ROOT",     NULL,       NULL,             0,            0,           0,         1,        -1 },
	{ "GLViewer",     NULL,       NULL,             0,            0,           0,         1,        -1 },
	{ "CENNS",     NULL,       NULL,             0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "H[]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define HYPERKEY Mod3Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
  // Basic stack/tag control
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },

  // Layouts
	{ MODKEY,			        XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */

  // Monitors
	{ MODKEY,			        XK_comma,	  focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_comma,	  tagmon,		{.i = -1 } },
	{ MODKEY,			        XK_period,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_period,	tagmon,		{.i = +1 } },

  // Quit, restart etc
	{ MODKEY|AltMask,			XK_q,		spawn,	SHCMD("kill -TERM $(pidof -s dwm)") },
	{ MODKEY|AltMask,			XK_r,		spawn,	SHCMD("kill -HUP $(pidof -s dwm)") },
	{ MODKEY|AltMask,			XK_l,		spawn,	SHCMD("i3lock -i/home/john/Pictures/sand.png") },
  { MODKEY|AltMask,     XK_s,   spawn,  SHCMD("notify-send \"Going to sleep...\"; systemctl suspend") },
  { MODKEY|AltMask,     XK_x,   spawn,  SHCMD("prompt \"Shutdown now?\" \"shutdown now\"") },

  // Misc window manager commands
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY,			XK_s,		togglesticky,	{0} },
	{ MODKEY,			XK_f,		togglefullscr,	{0} },
	{ MODKEY,			XK_h,		setmfact,	{.f = -0.05} },
	{ MODKEY,			XK_l,		setmfact,      	{.f = +0.05} },
	{ MODKEY,			XK_b,		togglebar,	{0} },
	{ MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} },


  // Media keys
	{ MODKEY,			XF86XK_AudioLowerVolume	,	spawn,		SHCMD("amixer sset Master 10%- ; pkill -RTMIN+10 dwmblocks") },
	{ MODKEY,			XF86XK_AudioRaiseVolume	,	spawn,		SHCMD("amixer sset Master 10%+ ; pkill -RTMIN+10 dwmblocks") },
	{ MODKEY,		  XF86XK_AudioMute,		      spawn,		SHCMD("amixer sset Master toggle ; pkill -RTMIN+10 dwmblocks") },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,		SHCMD("amixer sset Master 5%- ; pkill -RTMIN+10 dwmblocks") },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,		SHCMD("amixer sset Master 5%+ ; pkill -RTMIN+10 dwmblocks") },
	{ 0,	XF86XK_AudioMute,	        spawn,		SHCMD("amixer sset Master toggle ; pkill -RTMIN+10 dwmblocks") },
	{ 0,  XF86XK_AudioPrev,		      spawn,		SHCMD("mpc prev") },
	{ 0,  XF86XK_AudioNext,		      spawn,		SHCMD("mpc next") },
	{ 0,  XF86XK_AudioPause,		    spawn,		SHCMD("mpc pause") },
	{ 0,  XF86XK_AudioPlay,		      spawn,		SHCMD("mpc play") },
	{ 0,  XF86XK_AudioStop,		      spawn,		SHCMD("mpc stop") },
	{ 0,  XF86XK_AudioRewind,	      spawn,		SHCMD("mpc seek -10") },
	{ 0,  XF86XK_AudioForward,	    spawn,		SHCMD("mpc seek +10") },

  // Other function keys
	{ 0, XF86XK_MonBrightnessUp,	  spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },
  // TODO: keyboard backlight


  // Terminal
	{ MODKEY|ShiftMask,		XK_Return,	togglescratch,	{.ui = 0}       },
	{ MODKEY,			        XK_Return,	spawn,		      {.v = termcmd } },

  // App shortcuts
	{ HYPERKEY,			XK_b,		spawn,		SHCMD("$BROWSER")   },
	{ HYPERKEY,			XK_e,		spawn,		SHCMD("emacs")   },
	{ HYPERKEY,			XK_f,		spawn,		SHCMD("$FILE")   },
	{ HYPERKEY,			XK_d,		spawn,		SHCMD("discord")    },
	{ HYPERKEY,			XK_s,		spawn,		SHCMD("killall slack ; slack")    },
	{ HYPERKEY,			XK_t,		spawn,		SHCMD("toggletray") },

  // Classes
  { HYPERKEY,     XK_n,       spawn,    SHCMD("new_notes")  },
  { HYPERKEY,     XK_h,       spawn,    SHCMD("new_hw")  },
  { HYPERKEY,     XK_z,       spawn,    SHCMD("open_zoom")  },

  // Dmenu scripts
	{ MODKEY,			  XK_space,		spawn,    {.v = dmenucmd }      },
  { HYPERKEY,     XK_c,       spawn,    SHCMD("dmenuconfig")  },
  { HYPERKEY|ShiftMask,     XK_s,       spawn,    SHCMD("dmenuscripts") },
  { HYPERKEY|ShiftMask,     XK_e,       spawn,    SHCMD("dmenuunicode") },
  { HYPERKEY,     XK_g,       spawn,    SHCMD("dmenugeant")   },
  { HYPERKEY,     XK_slash,   spawn,    SHCMD("dmenusearch")  },
  { HYPERKEY|ShiftMask,     XK_b,   spawn,    SHCMD("dmenubooks")  },
  { HYPERKEY,     XK_m,       spawn,    SHCMD("dmenuman")  },
  { HYPERKEY,     XK_l,       spawn,    SHCMD("dmenulatex")  },
  { HYPERKEY,     XK_q,       spawn,    SHCMD("dmenubqn")  },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD("st -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
