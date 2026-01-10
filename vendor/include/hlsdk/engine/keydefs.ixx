export module keydefs;

//
// these are the key numbers that should be passed to Key_Event
//
export inline constexpr auto K_TAB = 9;
export inline constexpr auto K_ENTER = 13;
export inline constexpr auto K_ESCAPE = 27;
export inline constexpr auto K_SPACE = 32;

// normal keys should be passed as lowercased ascii

export inline constexpr auto K_BACKSPACE = 127;
export inline constexpr auto K_UPARROW = 128;
export inline constexpr auto K_DOWNARROW = 129;
export inline constexpr auto K_LEFTARROW = 130;
export inline constexpr auto K_RIGHTARROW = 131;

export inline constexpr auto K_ALT = 132;
export inline constexpr auto K_CTRL = 133;
export inline constexpr auto K_SHIFT = 134;
export inline constexpr auto K_F1 = 135;
export inline constexpr auto K_F2 = 136;
export inline constexpr auto K_F3 = 137;
export inline constexpr auto K_F4 = 138;
export inline constexpr auto K_F5 = 139;
export inline constexpr auto K_F6 = 140;
export inline constexpr auto K_F7 = 141;
export inline constexpr auto K_F8 = 142;
export inline constexpr auto K_F9 = 143;
export inline constexpr auto K_F10 = 144;
export inline constexpr auto K_F11 = 145;
export inline constexpr auto K_F12 = 146;
export inline constexpr auto K_INS = 147;
export inline constexpr auto K_DEL = 148;
export inline constexpr auto K_PGDN = 149;
export inline constexpr auto K_PGUP = 150;
export inline constexpr auto K_HOME = 151;
export inline constexpr auto K_END = 152;

export inline constexpr auto K_KP_HOME = 160;
export inline constexpr auto K_KP_UPARROW = 161;
export inline constexpr auto K_KP_PGUP = 162;
export inline constexpr auto K_KP_LEFTARROW = 163;
export inline constexpr auto K_KP_5 = 164;
export inline constexpr auto K_KP_RIGHTARROW = 165;
export inline constexpr auto K_KP_END = 166;
export inline constexpr auto K_KP_DOWNARROW = 167;
export inline constexpr auto K_KP_PGDN = 168;
export inline constexpr auto K_KP_ENTER = 169;
export inline constexpr auto K_KP_INS = 170;
export inline constexpr auto K_KP_DEL = 171;
export inline constexpr auto K_KP_SLASH = 172;
export inline constexpr auto K_KP_MINUS = 173;
export inline constexpr auto K_KP_PLUS = 174;
export inline constexpr auto K_CAPSLOCK = 175;


//
// joystick buttons
//
export inline constexpr auto K_JOY1 = 203;
export inline constexpr auto K_JOY2 = 204;
export inline constexpr auto K_JOY3 = 205;
export inline constexpr auto K_JOY4 = 206;

//
// aux keys are for multi-buttoned joysticks to generate so they can use
// the normal binding process
//
export inline constexpr auto K_AUX1 = 207;
export inline constexpr auto K_AUX2 = 208;
export inline constexpr auto K_AUX3 = 209;
export inline constexpr auto K_AUX4 = 210;
export inline constexpr auto K_AUX5 = 211;
export inline constexpr auto K_AUX6 = 212;
export inline constexpr auto K_AUX7 = 213;
export inline constexpr auto K_AUX8 = 214;
export inline constexpr auto K_AUX9 = 215;
export inline constexpr auto K_AUX10 = 216;
export inline constexpr auto K_AUX11 = 217;
export inline constexpr auto K_AUX12 = 218;
export inline constexpr auto K_AUX13 = 219;
export inline constexpr auto K_AUX14 = 220;
export inline constexpr auto K_AUX15 = 221;
export inline constexpr auto K_AUX16 = 222;
export inline constexpr auto K_AUX17 = 223;
export inline constexpr auto K_AUX18 = 224;
export inline constexpr auto K_AUX19 = 225;
export inline constexpr auto K_AUX20 = 226;
export inline constexpr auto K_AUX21 = 227;
export inline constexpr auto K_AUX22 = 228;
export inline constexpr auto K_AUX23 = 229;
export inline constexpr auto K_AUX24 = 230;
export inline constexpr auto K_AUX25 = 231;
export inline constexpr auto K_AUX26 = 232;
export inline constexpr auto K_AUX27 = 233;
export inline constexpr auto K_AUX28 = 234;
export inline constexpr auto K_AUX29 = 235;
export inline constexpr auto K_AUX30 = 236;
export inline constexpr auto K_AUX31 = 237;
export inline constexpr auto K_AUX32 = 238;
export inline constexpr auto K_MWHEELDOWN = 239;
export inline constexpr auto K_MWHEELUP = 240;

export inline constexpr auto K_PAUSE = 255;

//
// mouse buttons generate virtual keys
//
export inline constexpr auto K_MOUSE1 = 241;
export inline constexpr auto K_MOUSE2 = 242;
export inline constexpr auto K_MOUSE3 = 243;
export inline constexpr auto K_MOUSE4 = 244;
export inline constexpr auto K_MOUSE5 = 245;
