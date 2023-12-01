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
  uchar draw_property;
  uchar horizontal_speed_;
  float vertical_speed_;
};

// FORMAT : 0x<A><B><G><R>U
__constant unsigned colors[] = {0xFF000000U, 0xFFD0D0D0U, 0xFF00FFFFU, 0xFF333333U, 0xFFFF6600U, 0xFF16242CU, 0xFFEFEFE8U, 0xFF0000FFU, 0xFF383B3BU};

__constant unsigned sand_colors[] = {0xFF69A9D8U, 0xFF7ABCEAU, 0xFF90C7F4U, 0xFF7ABCEAU};
__constant unsigned stone_colors[] = {0xFF656363U, 0xFF6C6766U, 0xFF535254U, 0xFF666464U};

__kernel void PaintOn(__constant struct Element elements[WORLD_SIZE_Y][WORLD_SIZE_X],
                      __global unsigned pixels[SCREEN_SIZE_Y][SCREEN_SIZE_X], const float view_left,
                      const float view_top, const float view_step_x, const float view_step_y) {
  const int2 screen = {get_global_id(0), get_global_id(1)};

  const int2 world = {view_left + screen.x * view_step_x, view_top + screen.y * view_step_y};

  const uchar subs = elements[world.y][world.x].subs;

  switch (subs) {
    case 2: { // Sand
      pixels[screen.y][screen.x] = sand_colors[elements[world.y][world.x].draw_property];
      break;
    }
    case 3: { // Stone
      pixels[screen.y][screen.x] = stone_colors[elements[world.y][world.x].draw_property];
      break;
    }
    default: {
      pixels[screen.y][screen.x] = colors[elements[world.y][world.x].subs];
    }
  }
}
