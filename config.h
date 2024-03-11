#ifndef CONFIG
#define CONFIG

const char *touchpad_path = "/dev/input/event6";
const char *on_heart_detect_command = "gwenview /home/dawidogg/Pictures/gimp/pelmeni.png >nul 2>nul &";
const int skip_writing = 1;
const char *output_dir = "data_empty";
const int ignore_first = 8;

#endif // CONFIG
