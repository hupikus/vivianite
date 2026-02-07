#include "vivianite.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <boost/dll/alias.hpp>

void init()
{
}

void draw(SDL_Renderer *r, const SDL_Rect& rect)
{
	const int &w = rect.w;
	const int &h = rect.h;
	SDL_RenderFillRect(r, new (const SDL_FRect){w*0.25f, h*0.25f, w*0.75f, h*0.75f});

	float scale = (float)rect.w * 0.9f / (float)(26*8);
	SDL_SetRenderScale(r, scale, scale);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	SDL_RenderDebugText(
		r,
		(rect.w * 0.05f) / scale,
		(rect.h / 2.0f - 4.0f * scale) / scale,
		"Not failed to load plugin."
	);
}

void destroy()
{
}

// Required
BOOST_DLL_ALIAS(init, _init)
BOOST_DLL_ALIAS(draw, _draw)
BOOST_DLL_ALIAS(destroy, _destroy)
