#if !defined(LUDUM_ASSETS_H_)
#define LUDUM_ASSETS_H_

struct Image_Handle {
    u32 index;
};

struct Sound_Handle {
    u32 index;
};

struct Music_Handle {
    u32 index;
};

struct Font_Handle {
    u32 index;
};

enum Asset_Type {
    AssetType_None = 0,
    AssetType_Image,
    AssetType_Sound,
    AssetType_Music,
    AssetType_Font
};

struct Asset {
    Asset_Type type;
    union {
        sfSoundBuffer *sound;
        sfTexture *image;
        sfMusic   *music;
        sfFont    *font;
    };
};

struct Asset_Manager {
    Asset *assets;
    u32 asset_count;
    u32 max_asset_count;
};

inline bool IsValid(Image_Handle handle) {
    bool result = (handle.index != 0);
    return result;
}

inline bool IsValid(Sound_Handle handle) {
    bool result = (handle.index != 0);
    return result;
}

inline bool IsValid(Music_Handle handle) {
    bool result = (handle.index != 0);
    return result;
}

inline bool IsValid(Font_Handle handle) {
    bool result = (handle.index != 0);
    return result;
}

internal Image_Handle LoadImage(Asset_Manager *manager, char *path);
internal Sound_Handle LoadSound(Asset_Manager *manager, char *path);
internal Music_Handle LoadMusic(Asset_Manager *manager, char *path);
internal Font_Handle  LoadFont(Asset_Manager *manager, char *path);

internal sfSoundBuffer *GetSound(Asset_Manager *manager, Sound_Handle handle);
internal sfTexture *GetImage(Asset_Manager *manager, Image_Handle handle);
internal sfMusic   *GetMusic(Asset_Manager *manager, Music_Handle handle);
internal sfFont    *GetFont(Asset_Manager *manager, Font_Handle handle);

#endif  // LUDUM_ASSETS_H_
