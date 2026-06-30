#include "vivianite.h"

#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <math.h>

char percentage[64] = {0};

const char *texts[] = {
    "Plugin loaded.",
    "Video initialized.",
    "FFMpeg started",
    "Starting render",
    &(percentage[0]),
    "Render finished.",
};
int textProg = 1;

videoCache *video;

SDL_Texture *t;
SDL_Renderer *r;
SDL_Surface *s;

videoSpecs specs;

extern "C" void tile_init()
{
    snprintf(percentage, 64, "Rendering is still starting.");

    specs = videoSpecs();
    specs.file_name = "output.mp4";
    specs.width = 1280;
    specs.height = 1280;
    specs.frame_rate = 60;
    video = InitVideo(specs);
    textProg = 2;
    
}


int frame_delay = 1;
int frame_count = 0;
const int frame_target = 360;
const int max_frames = 2;

void render() {
    const float cx = (float)specs.width / 2.0f;
    const float cy = (float)specs.height / 2.0f;
    const float radius = 300.0f;
    const float size = 100.0f;

    SDL_FRect rect = {0, 0, size, size};

    int cr_frames = 0;

    while (textProg == 5 && cr_frames++ <= max_frames) {

	// Clean surface before
	if (s != NULL) {
	    SDL_DestroySurface(s);
	    s = NULL;
	}

	SDL_SetRenderTarget(r, t);

	SDL_SetRenderDrawColor(r, 145, 35, 155 - (frame_count / 3), 255);
	SDL_RenderFillRect(r, NULL);


	{
	    rect.x = cx + cosf(frame_count / 20.0f * M_PI) * radius - 15.0f;
	    rect.y = cy + sinf(frame_count / 20.0f * M_PI) * radius * 1.3f;

	    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	    SDL_RenderFillRect(r, &rect);
	}



	s = SDL_RenderReadPixels(r, NULL);
	SendFrame(video, s->pixels);

	SDL_SetRenderTarget(r, NULL);

	if (frame_count++ >= frame_target) {
	    EndRender(video);
	    textProg = 6;
	}
    }

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SendRenderPreview(video, t);
}

extern "C" void tile_draw(SDL_Renderer *renderer, int w, int h)
{

    int size = 32;
    int x = 0;
    int y = 0;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < textProg; ++i) {
	SDL_RenderDebugText(renderer, 0, i * 12, texts[i]);

    }


    // Draw
    if (textProg == 2) {
	r = renderer;
	t = CreateTexture(r, specs.width, specs.height);

	StartRender(video);
	textProg = 3;
	snprintf(percentage, 64, "Rendering is still starting.");
    } else if (textProg == 3) {
	if (frame_count++ >= 260) {
	    frame_count = 0;
	    textProg = 5;
	}
    } else if (textProg == 5) {
	snprintf(percentage, 64, "Rendering is done by %f%%.", ((float)frame_count / (float)frame_target * 100.0f));
	if (frame_delay <= 0) {
	    render();
	    frame_delay = 1;
	} else {
	    frame_delay -= 1;
	}
    }

 }

extern "C" void tile_destroy()
{
    EndRender(video);
    SDL_DestroySurface(s);
}
