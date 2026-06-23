#ifndef VIVIANITE_H
#define VIVIANITE_H

#include <string>
#include <SDL3/SDL.h>

// Log
extern void Alert(const std::string &msg);

// No Config!
//extern Config *read_config(Tile *tile, const std::string)

// Keys
extern bool IsKeyPressed(int key);
extern bool IsKeyDown(int key);
extern bool IsKeyReleased(int key);

// Resources

// Text
#ifdef VIVIANITE_FT
#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Face LoadFont(const std::string &path);
extern bool IsFontLoaded(const std::string &path);

extern FT_Face GetFont(void);
extern FT_Face GetDefaultFont(void);

extern void SetRenderFont(const FT_Face);
#endif // VIVIANITE_FT

// Shapes
extern void DrawRectangle();

// Render
typedef struct videoSpecs {
	const char *name;
	size_t width, height = 0.0f;
	float framerate = 60.0f;

	int channels = 1;
	size_t freq = 44100;
	size_t depth = 16;
} videoSpecs;
struct videoCache;
extern videoCache *InitVideo(videoSpecs specs);

extern void SendFrame(videoCache *video, void *frame);
extern void SendAudio(videoCache *video, void *samples);

typedef struct exportOptions {
	const char *format;
} exportOptions;
extern void ExportVideo(videoCache *video, exportOptions options);

#endif // VIVIANITE_H
