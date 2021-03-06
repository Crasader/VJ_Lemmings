#ifndef _COMMON_DEFS_INCLUDE
#define _COMMON_DEFS_INCLUDE


#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 210
#define colorGreen glm::vec4(0,1,0,1)
#define colorWhite glm::vec4(1,1,1,1)
#define colorBlack glm::vec4(0,0,0,1)
#define colorRed glm::vec4(1,0,0,1)
#define colorBlue glm::vec4(0,0,1,1)
#define colorYellow glm::vec4(1,1,0,1)
#define colorMagenta glm::vec4(1,0,1,1)
#define colorCian glm::vec4(0,1,1,1)
#define colorDarkGreen glm::vec4(0, 0.33f, 0, 1)
#define colorOrange glm::vec4(1,0.5f,0,1)
#define textRPos (CAMERA_HEIGHT * 3 / 2) - 32
#define speed 12
#define speedDoorStart 8
#define speedDoorEnd 8
#define spawnTime 5000

enum PixelFormat {TEXTURE_PIXEL_FORMAT_RGB, TEXTURE_PIXEL_FORMAT_RGBA, TEXTURE_PIXEL_FORMAT_L};


#endif //_COMMON_DEFS_INCLUDE


