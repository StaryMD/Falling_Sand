#ifndef SCREEN_SIZE_X
#define SCREEN_SIZE_X 1
#endif

#ifndef SCREEN_SIZE_Y
#define SCREEN_SIZE_Y 1
#endif

#ifndef WORLD_SIZE_X
#define WORLD_SIZE_X 1
#endif

#ifndef WORLD_SIZE_Y
#define WORLD_SIZE_Y 1
#endif

struct Element {
  uchar subs;
  // uchar color_sample;
  uchar horizontal_speed_;
  float vertical_speed_;
};

// FORMAT : 0x<A><B><G><R>U
__constant unsigned colors[] = {0xFF000000U, 0xFFD0D0D0U, 0xFF00FFFFU, 0xFF333333U, 0xFFFF6600U, 0xFF16242CU, 0xFFEFEFE8U, 0xFF0000FFU, 0xFF383B3BU};

__kernel void PaintOn(__constant struct Element elements[WORLD_SIZE_Y][WORLD_SIZE_X],
                      __global unsigned pixels[SCREEN_SIZE_Y][SCREEN_SIZE_X], const float view_left,
                      const float view_top, const float view_step_x, const float view_step_y) {
  const int2 screen = {get_global_id(0), get_global_id(1)};

  const int2 world = {view_left + screen.x * view_step_x, view_top + screen.y * view_step_y};

  pixels[screen.y][screen.x] = colors[elements[world.y][world.x].subs];
}
