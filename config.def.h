/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int gappx     = 0;        /* gaps between windows */
static unsigned int snap      = 0;        /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]    = { "monospace:size=10" };
static const int refreshrate  = 60;       /* Update rate for drag and resize events, in updates (frames) per second */
static int vertpad            = 0;        /* vertical padding of bar */
static int sidepad            = 0;        /* horizontal padding of bar */
static int horizpadbar        = 2;        /* horizontal padding for statusbar */
static int vertpadbar         = 0;        /* vertical padding for statusbar */
static char normbg[]          = "#222222";
static char normborder[]      = "#444444";
static char normfg[]          = "#bbbbbb";
static char selfg[]           = "#eeeeee";
static char selbg[]           = "#005577";
static char urgbg[]       = "#cc3333";
static char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfg,    normbg,    normborder },
	[SchemeSel]  = { selfg,     selbg,     selbg  },
	[SchemeUrg]  = { selfg,     urgbg,     urgbg  },
};

/* centered title */
static unsigned int centeredtitle = 1;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                        instance    title            tags mask     isfloating   monitor */
	{ "Nsxiv",                      NULL,       NULL,            0,            1,           -1 },
	{ "mpv",                        NULL,       NULL,            1 << 8,       0,           -1 },
	{ "ffplay",                     NULL,       NULL,            1 << 8,       1,           -1 },
	{ "Nm-connection-editor",       NULL,       NULL,            0,            1,           -1 },
	{ NULL,                         NULL,       "><>",           0,            1,           -1 },
	{ NULL,                         NULL,       "Alsa Mixer",    0,            1,           -1 },
	{ NULL,                         NULL,       "Record Screen", 1 << 8,       1,           -1 },

	{ "firefox-developer-edition",  NULL,       NULL,            1 << 1,       0,           -1 },
	{ "krita",                      NULL,       NULL,            1 << 2,       0,           -1 },
	{ "Postman",                    NULL,       NULL,            1 << 3,       0,           -1 },
	{ "dbgate",                     NULL,       NULL,            1 << 3,       0,           -1 },
	{ NULL,                         NULL,       "File Manager",  1 << 2,       0,           -1 },
};

/* layout(s) */
static float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster        = 1;    /* number of clients in master area */
static int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static char statusbar[] = "dwmblocks";

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "borderpx",       XresInteger,    &borderpx },
	{ "gappx",          XresInteger,    &gappx },
	{ "snap",           XresInteger,    &snap },
	{ "showbar",        XresInteger,    &showbar },
	{ "topbar",         XresInteger,    &topbar },
	{ "vertpad",        XresInteger,    &vertpad },
	{ "sidepad",        XresInteger,    &sidepad },
	{ "horizpadbar",    XresInteger,    &horizpadbar },
	{ "vertpadbar",     XresInteger,    &vertpadbar },
	{ "font",           XresStringPtr,  &fonts[0] },
	{ "normbg",         XresString,     &normbg },
	{ "normfg",         XresString,     &normfg },
	{ "normborder",     XresString,     &normborder },
	{ "selbg",          XresString,     &selbg },
	{ "selfg",          XresString,     &selfg },
	{ "urgbg",          XresString,     &urgbg },
	{ "mfact",          XresFloat,      &mfact },
	{ "nmaster",        XresInteger,    &nmaster },
	{ "resizehints",    XresInteger,    &resizehints },
	{ "lockfullscreen", XresInteger,    &lockfullscreen },
	{ "statusbar",      XresString,     &statusbar },
	{ "centeredtitle",  XresInteger,    &centeredtitle },
};

static const Key keys[] = {
	/* modifier                     key                       function        argument */
	/* { MODKEY|ControlMask|ShiftMask, XK_q,                     quit,           {0} }, */
	{ MODKEY,                       XK_x,                     spawn,          {.v = (const char *[]){ "xkill", NULL } } },
	{ MODKEY,                       XK_0,                     spawn,          SHCMD("$HOME/bin/dmenupower.sh") },

	{ MODKEY,                       XK_Return,                spawn,          SHCMD("st -T 'tmux' -e $HOME/bin/launch_tmux.sh") },
	{ MODKEY|ShiftMask,             XK_Return,                spawn,          {.v = (const char *[]){ "st", "-T", "><>", NULL } } },
	{ MODKEY|ControlMask|ShiftMask, XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_d,                     spawn,          SHCMD("$HOME/bin/dmenudesktop.sh") },
	{ MODKEY|ShiftMask,             XK_d,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_F2,                    spawn,          {.v = (const char *[]){ "firefox-developer-edition", NULL } } },
	{ MODKEY,                       XK_F3,                    spawn,          {.v = (const char *[]){ "st", "-T", "'File Manager'", "-e", "vifm", NULL } } },
	{ MODKEY,                       XK_a,                     spawn,          {.v = (const char *[]){ "st", "-T", "'Alsa Mixer'", "-e", "alsamixer", NULL } } },
	{ MODKEY,                       XK_Tab,                   spawn,          {.v = (const char *[]){ "networkmanager_dmenu", NULL } } },
	{ MODKEY,                       XK_equal,                 spawn,          {.v = (const char *[]){ "=", NULL } } },
	{ MODKEY,                       XK_n,                     spawn,          SHCMD("$HOME/bin/dmenumount.sh") },

	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5% && pkill -RTMIN+5 dsblocks") },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5% && pkill -RTMIN+5 dsblocks") },
	{ 0,                            XF86XK_AudioMute,         spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && pkill -RTMIN+5 dsblocks") },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,  spawn,          SHCMD("pactl set-source-volume @DEFAULT_SOURCE@ +5% && pkill -RTMIN+4 dsblocks") },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,  spawn,          SHCMD("pactl set-source-volume @DEFAULT_SOURCE@ -5% && pkill -RTMIN+4 dsblocks") },
	{ 0,                            XF86XK_AudioMicMute,      spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle && pkill -RTMIN+4 dsblocks") },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn,          SHCMD("xbacklight -inc 5 && pkill -RTMIN+3 dsblocks") },
	{ 0,                            XF86XK_MonBrightnessDown, spawn,          SHCMD("xbacklight -dec 5 && pkill -RTMIN+3 dsblocks") },
	{ MODKEY,                       XK_F5,                    spawn,          {.v = (const char *[]){ "playerctl", "play-pause", NULL } } },

	{ MODKEY,                       XK_Print,                 spawn,          SHCMD("scrot -z $HOME/Pictures/screenshot/$(date +%s).png; notify-send 'screenshot saved'") },
	{ MODKEY|ControlMask,           XK_Print,                 spawn,          SHCMD("scrot -z -o -s -f $HOME/Pictures/screenshot/$(date +%s).png; notify-send 'screenshot saved'") },
	{ MODKEY|ShiftMask,             XK_Print,                 spawn,          SHCMD("scrot -z -o -s -f --format png /dev/stdout | xclip -selection clipboard -t image/png -i; notify-send 'screenshot copied to clipboard'") },
	{ MODKEY|Mod1Mask,              XK_Print,                 spawn,          SHCMD("st -T 'Record Screen' -e $HOME/bin/record.sh") },

	{ MODKEY,                       XK_q,                     killclient,     {0} },
	{ MODKEY|ControlMask,           XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_grave,                 view,           {0} },
	{ MODKEY,                       XK_z,                     view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_p,                     tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },

	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,                     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,                     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} },

	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_u,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                 togglefloating, {0} },
	{ MODKEY,                       XK_f,                     togglefullscr,  {0} },

	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                tagmon,         {.i = +1 } },

	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

