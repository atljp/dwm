/* See LICENSE file for copyright and license details. */
/* Bind are made for German keyboard layout */

#include <X11/XF86keysym.h>

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "[D]",      deck },
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,	      NULL },

};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \
/*	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
/*	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
/*	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
/*	{ MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key       	 function        argument */
	{ MODKEY,                       XK_d,     	 spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return,	 spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,     	 togglebar,      {0} },
	{ MODKEY,                       XK_o,     	 incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,     	 incnmaster,     {.i = -1 } },
	STACKKEYS(MODKEY,                         	 focus)
	STACKKEYS(MODKEY|ShiftMask,               	 push)
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,                       XK_h,     	 setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,     	 setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,	 zoom,           {0} },
	{ MODKEY,                       XK_Tab,   	 view,           {0} },
	{ MODKEY,  	                XK_q,     	 killclient,     {0} },
	{ MODKEY,	                XK_f,     	 togglefullscr,  {0} },
	{ MODKEY, 		        XK_s,     	 togglesticky,   {0} },
	{ MODKEY,                       XK_t,     	 setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,	  	 setlayout,	   {.v = &layouts[1]} }, /* bstack */ 
	{ MODKEY,                       XK_z,     	 setlayout,      {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,             XK_z,     	 setlayout,      {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,                       XK_u,     	 setlayout,      {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,             XK_u,     	 setlayout,      {.v = &layouts[5]} }, /* monocle */
	{ MODKEY,                       XK_i,     	 setlayout,      {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,             XK_i,     	 setlayout,      {.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY|ShiftMask,		XK_f,	  	 setlayout,      {.v = &layouts[8]} }, /* floating */
	{ MODKEY,	                XK_y,     	 incrgaps,       {.i = +3 } },
	{ MODKEY, 		        XK_x,     	 incrgaps,       {.i = -3 } },
	{ MODKEY,	                XK_a,     	 togglegaps,     {0} },
	{ MODKEY|ShiftMask,	        XK_a,     	 defaultgaps,    {0} },
	{ MODKEY,                       XK_space, 	 setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space, 	 togglefloating, {0} },
	{ MODKEY,                       XK_0,     	 view,           {.ui = ~0 } }, /* view all windows regardless of tag */
	{ MODKEY|ShiftMask,             XK_0,     	 tag,            {.ui = ~0 } }, /* put current window on all tags */
	{ MODKEY,                       XK_comma, 	 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,	 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma, 	 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,	 tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,     	                 0)
	TAGKEYS(                        XK_2,     	                 1)
	TAGKEYS(                        XK_3,     	                 2)
	TAGKEYS(                        XK_4,     	                 3)
	TAGKEYS(                        XK_5,     	                 4)
	TAGKEYS(                        XK_6,     	                 5)
	TAGKEYS(                        XK_7,     	                 6)
	TAGKEYS(                        XK_8,     	                 7)
	TAGKEYS(                        XK_9,     	                 8)
	{ MODKEY|ShiftMask,             XK_q,     	 quit,           {0} },
	{ MODKEY,			XK_BackSpace,	 quit,       	 {1} }, /* reload */ 
	{ MODKEY, 			XK_dead_acute,	 spawn,		 {.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY, 			XK_ssharp,	 spawn,		 {.v = (const char*[]){ "xbacklight", "-set", "0", NULL } } }, /* turn off laptop display */
	{ MODKEY, 			XK_udiaeresis,	 spawn,		 {.v = (const char*[]){ "displayselect", NULL } } },
	{ MODKEY, 			XK_odiaeresis,	 spawn,		 {.v = (const char*[]){ "pavucontrol", NULL } } },
	{ MODKEY|ShiftMask,		XK_odiaeresis,	 spawn,		 {.v = (const char*[]){ TERMINAL, "-e", "pulsemixer", NULL } } },
	{ MODKEY, 			XK_adiaeresis,	 spawn,		 {.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } }, /* NetworkManager package */
/*	{ 0, XF86XK_MonBrightnessUp,    spawn,		{.v = (const char*[]){ "xbacklight", "-inc", "10", NULL } } }, */
/*	{ 0, XF86XK_MonBrightnessDown,  spawn,		{.v = (const char*[]){ "xbacklight", "-dec", "10", NULL } } }, */
	{ 0, XF86XK_ScreenSaver,	spawn,		{.v = (const char*[]){ "sysact", NULL } } }, /* x220 */
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SOURCE@ toggle") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%- && wpctl set-volume @DEFAULT_AUDIO_SINK@ 2%+; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%+ && wpctl set-volume @DEFAULT_AUDIO_SINK@ 2%-; kill -44 $(pidof dwmblocks)") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText|ShiftMask,        0,              Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

