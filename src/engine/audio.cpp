#include <engine/audio.hpp>

std::shared_ptr<Music> Music::FromFile(const std::string& path)
{
    auto* music = Mix_LoadMUS(path.c_str());

    if (music == nullptr)
    {
        return nullptr;
    }

    auto out = std::make_shared<Music>();
    out->handle = decltype(out->handle) { music };
    return out;
}

void MusicChannel::Init()
{
    Mix_HookMusicFinished(MusicChannel::TrackEndCallback);
}

void MusicChannel::Shutdown()
{
    Mix_HaltMusic();
    Mix_HookMusicFinished(nullptr);

    current_entry = {};
    playlist = {};
}

void MusicChannel::PushTrack(const PlaylistEntry& track)
{
    std::scoped_lock<std::mutex> lock { queue_mutex };

    if (current_entry.music_handle == nullptr)
    {
        current_entry = track;

        Mix_FadeInMusic(
            current_entry.music_handle->GetHandle(),
            -1,
            current_entry.fade_in_length.count() * 10.0f);
    }
    else
    {
        playlist.push(track);
    }
}

void MusicChannel::NextTrack(bool skip_fadeout)
{
    std::scoped_lock<std::mutex> lock { queue_mutex };
    if (skip_fadeout)
    {
        Mix_HaltMusic();
    }
    else
    {
        Mix_FadeOutMusic(current_entry.fade_out_length.count() * 10.0f);
    }
}
void MusicChannel::TrackEndCallback()
{
    std::scoped_lock<std::mutex> lock { queue_mutex };

    if (playlist.empty())
    {
        current_entry = {};
        return;
    }

    current_entry = playlist.front();
    playlist.pop();

    Mix_FadeInMusic(
        current_entry.music_handle->GetHandle(),
        -1,
        current_entry.fade_in_length.count() * 10.0f);
}
