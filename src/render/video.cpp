#include "vivianite.h"

#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

// A significant chunk of code adapted from https://github.com/tsoding/rendering-video-in-c-with-ffmpeg/blob/master/ffmpeg_linux.c for testing.

#define READ_END 0
#define WRITE_END 1

typedef struct jobs {
    videoCache **data;
    size_t count;
    size_t capacity;
} jobs;

videoCache *current_job = NULL;
videoCache *GetActiveJob(void) { return current_job; }

videoCache *InitVideo(videoSpecs specs)
{
    if (specs.width == 0 || specs.height == 0 || specs.frame_rate <= 0.0f) return NULL;

    if (specs.channels > 1)
        fprintf(stderr,
            "Channels value %zu: "
            "support for value higher than 1 is not implemented.\n",
            specs.channels
        );
    if (specs.channels <= 0) specs.channels = 1;

    if (specs.save_frames)
        fprintf(stderr, "Saving frames is not implemented.\n");

    videoCache *video = new videoCache();

    video->specs = new videoSpecs;
    *(video->specs) = specs;

    video->width = specs.width;
    video->height = specs.height;
    video->color_depth = specs.color_depth;

    video->rendering = false;

    return video;
}

int StartRender(videoCache *video)
{
    videoSpecs *specs = video->specs;
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        fprintf(stderr, "ERROR: failed to create a pipe: %s.\n", strerror(errno));
        return -1;
    }


    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "ERROR: failed to create a process: %s.\n", strerror(errno));
        return -1;
    }

    if (child == 0) {
        if (dup2(pipefd[READ_END], STDIN_FILENO) < 0) {
            fprintf(stderr, "ERROR: failed to reopen read end of pipe as stdin: %s\n", strerror(errno));
            return -1;
        }
        close(pipefd[WRITE_END]);

        char res[64];
        snprintf(res, 64, "%zux%zu", specs->width, specs->height);
        char framerate[64];
        snprintf(framerate, 64, "%f", specs->frame_rate);

        char video_bitrate[64];
        if (specs->video_bitrate != NULL) {
            snprintf(video_bitrate, 64, "%s", specs->video_bitrate);
        } else {
            snprintf(
                video_bitrate, 64, "%zu",
                (size_t)(specs->width * specs->height * specs->frame_rate * specs->color_depth) + 1
            );
        }

        char audio_bitrate[64];
        if (specs->audio_bitrate != NULL) {
            snprintf(audio_bitrate, 64, "%s", specs->audio_bitrate);
        } else {
            snprintf(
                audio_bitrate, 64, "%zu",
                (specs->sample_rate * specs->audio_depth)
            );
        }

        // Execute ffmpeg
        int ret = execlp(
            "ffmpeg",
            "ffmpeg",
            "-loglevel", "verbose",
            "-y",

            "-f", "rawvideo",
            "-pix_fmt", "rgba",
            "-s", res,
            "-r", framerate,
            "-i", "-",

            "-c:v", specs->video_codec,
            "-vb", video_bitrate,
            "-c:a", specs->audio_codec,
            "-ab", audio_bitrate,
            "-pix_fmt", "yuv420p",
            specs->file_name,

            NULL
        );

        if (ret < 0) {
            fprintf(stderr, "ERROR: failed to run ffmpeg as a child process: %s\n", strerror(errno));
            return -1;
        }


    }

    video->rendering = true;
    video->ffmpeg = new PIPE();
    video->ffmpeg->pid = child;
    video->ffmpeg->pipe = pipefd[WRITE_END];

    current_job = video;
    return 0;
}


void SendFrame(videoCache *video, void *frame)
{
    void *data = frame; // ENCODE IF NEEDED, NOT IMPLEMENTED
    write(video->ffmpeg->pipe, data, (size_t)(video->color_depth / 8) * video->width * video->height);

    ++video->frames;
}

void SendRenderPreview(videoCache *video, SDL_Texture *t)
{
    if (video->frame_preview) {
        SDL_DestroyTexture(video->frame_preview);
    }
    video->frame_preview = t;
}

SDL_Texture *GetRenderPreview(videoCache *video) { return video->frame_preview; }


void SendAudio(videoCache *video, void *samples)
{

}

void CancelRender(videoCache *video)
{
    if (video == NULL) return;
    video->rendering = false;
    current_job = NULL;

    if (video->ffmpeg != NULL) {
        if (video->ffmpeg->pipe != 0)
            close(video->ffmpeg->pipe);
        if (video->ffmpeg->pid != 0)
            kill(video->ffmpeg->pid, SIGINT);
        free(video->ffmpeg);
    }

    printf("Aborting render of %s.\n", video->specs->file_name);
}

void EndRender(videoCache *video)
{
    if (video->rendering) {
        video->rendering = false;
        current_job = NULL;

        if (video->ffmpeg != NULL) {
            close(video->ffmpeg->pipe);
            waitpid(video->ffmpeg->pid, NULL, 0);
            free(video->ffmpeg);
        }

        printf("Stopping render of %s.\n", video->specs->file_name);
    }
}
