#include <SDL3_mixer/SDL_mixer.h>
#include <utility/time.hpp>
#include <utility/memory.hpp>
#include <mutex>
#include <queue>

class Music
{
public:
    static std::shared_ptr<Music> FromFile(const std::string& path);
    Mix_Music* GetHandle() const { return handle.get(); }

private:
    utility::unique_ptr<Mix_Music, Mix_FreeMusic> handle {};
};

struct PlaylistEntry
{
    std::shared_ptr<Music> music_handle {};
    DeltaMS fade_in_length {};
    DeltaMS fade_out_length {};
};

class MusicChannel
{
public:
    static void Init();
    static void Shutdown();

    static void PushTrack(const PlaylistEntry& track);
    static void NextTrack(bool skip_fadeout);

private:
    static SDLCALL void TrackEndCallback();

    static inline std::mutex queue_mutex {};
    static inline std::queue<PlaylistEntry> playlist {};
    static inline PlaylistEntry current_entry {};
};