#ifndef SCREEN_SIZE_X
#error "SCREEN_SIZE_X not defined"
#endif

#ifndef SCREEN_SIZE_Y
#error "SCREEN_SIZE_Y not defined"
#endif

#ifndef WORLD_SIZE_X
#error "WORLD_SIZE_X not defined"
#endif

#ifndef WORLD_SIZE_Y
#error "WORLD_SIZE_Y not defined"
#endif

struct Element {
  uchar subs;
  uchar draw_property;
  uchar horizontal_speed_;
  float vertical_speed_;
};

// FORMAT : 0x<A><B><G><R>U
__constant unsigned colors[] = {0, 0xFFD0D0D0U, 0, 0, 0, 0, 0, 0, 0xFF383B3BU};

__constant unsigned sand_colors[] = {0xFF69A9D8U, 0xFF7ABCEAU, 0xFF90C7F4U, 0xFF7ABCEAU};
__constant unsigned stone_colors[] = {0xFF656363U, 0xFF6C6766U, 0xFF535254U, 0xFF666464U};
__constant unsigned water_colors[] = {0xFF982804U, 0xFFAA3105U, 0xFF9D2A05U, 0xFF902503U};
__constant unsigned oil_colors[] = {0xFF3FAACBU, 0xFF3BA7C9U, 0xFF36A4C8U, 0xFF339FC1U};
__constant unsigned steam_colors[] = {0xFFE4DFDCU, 0xFFE8E3E1U, 0xFFEDE8E5U, 0xFFDFDAD7U};
__constant unsigned fire_colors[] = {0xFF0075FFU, 0xFF0064FCU, 0xFF0235D7U, 0xFF0322B6U};
__constant unsigned smoke_colors[] = {0xFF090909U, 0xFF0A0A0AU, 0xFF0C0C0CU, 0xFF0A0A0AU};

__kernel void PaintOn(__global struct Element elements[WORLD_SIZE_Y][WORLD_SIZE_X],
                      __global unsigned pixels[SCREEN_SIZE_Y][SCREEN_SIZE_X],
                      const float view_left, const float view_top,
                      const float view_step_x, const float view_step_y,
                      const unsigned tick_count) {
  const int2 screen = {get_global_id(0), get_global_id(1)};

  const int2 world = {view_left + screen.x * view_step_x, view_top + screen.y * view_step_y};

  const uchar subs = elements[world.y][world.x].subs;

  const uchar draw_property = elements[world.y][world.x].draw_property;

  switch (subs) {
    case 2: { // Sand
      pixels[screen.y][screen.x] = sand_colors[draw_property % 4];
      break;
    }
    case 3: { // Stone
      pixels[screen.y][screen.x] = stone_colors[draw_property % 4];
      break;
    }
    case 4: { // Water
      const unsigned index = ((tick_count + draw_property) / 20) % 4;
      pixels[screen.y][screen.x] = water_colors[index];
      break;
    }
    case 5: { // Oil
      const unsigned index = ((tick_count + draw_property) / 50) % 4;
      pixels[screen.y][screen.x] = oil_colors[index];
      break;
    }
    case 6: { // Water
      const unsigned index = ((tick_count + draw_property) / 30) % 4;
      pixels[screen.y][screen.x] = steam_colors[index];
      break;
    }
    case 7: { // Fire
      const unsigned index = ((tick_count + draw_property) / 40) % 4;
      pixels[screen.y][screen.x] = fire_colors[index];
      break;
    }
    case 8: { // Smoke
      const unsigned index = ((tick_count + draw_property) / 40) % 4;
      pixels[screen.y][screen.x] = smoke_colors[index];
      break;
    }
    default: {
      pixels[screen.y][screen.x] = colors[subs];
    }
  }
}
