#ifndef VIVIANITE_H
#define VIVIANITE_H

#include <string>
#include <stdbool.h>
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

// Render
extern SDL_Texture *CreateTexture(SDL_Renderer *r, size_t width, size_t height);

extern void DrawRectangle(SDL_Texture *t, float x, float y, float width, float height);

// Export
typedef struct videoSpecs {
	const char *file_name = "output.mp4";
	
	size_t width, height;
	float frame_rate = 60;
	size_t color_depth = 32;

	size_t channels = 1;
	size_t sample_rate = 44100;
	size_t audio_depth = 16;


	const char *video_codec = "libx264";
	const char *audio_codec = "aac";

	const char *video_bitrate = NULL;
	const char *audio_bitrate = NULL;

	bool save_frames = false;
	char *frames_location;
} videoSpecs;

typedef struct videoCache videoCache;
extern videoCache *InitVideo(videoSpecs specs);

extern int StartRender(videoCache *video);
extern void SendFrame(videoCache *video, void *frame);
extern void SendAudio(videoCache *video, void *samples);
extern void EndRender(videoCache *video);
extern void CancelRender(videoCache *video);

// Video Cache definitions
typedef struct pipe_data {
	int pid;
	int pipe;
} PIPE;

struct videoCache {
	size_t width, height;
	size_t color_depth;

	videoSpecs *specs;
	PIPE *ffmpeg;
	size_t frames;

	bool rendering;
};

#endif // VIVIANITE_H
