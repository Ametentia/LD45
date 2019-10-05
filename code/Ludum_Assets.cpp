internal void InitialiseAssetManager(Asset_Manager *manager, u32 max_asset_count) {
    manager->asset_count = 1;
    manager->max_asset_count = max_asset_count;
    manager->assets = cast(Asset *) malloc(sizeof(Asset) * max_asset_count);
}

internal Asset *GetAsset(Asset_Manager *manager, u32 index) {
    Assert(index < manager->max_asset_count);
    Asset *result = &manager->assets[index];

    return result;
}

internal Image_Handle LoadImage(Asset_Manager *manager, char *path) {
    Image_Handle result = {};
    if ((manager->asset_count + 1) < manager->max_asset_count) {
        result.index = manager->asset_count + 1;
        Asset *asset = GetAsset(manager, result.index);

        asset->image = sfTexture_createFromFile(path, 0);

        if (!asset->image) { result.index = 0; }
        else {
            sfTexture_setSmooth(asset->image, true);
            sfTexture_generateMipmap(asset->image);

            asset->type = AssetType_Image;
            ++manager->asset_count;
        }
    }

    return result;
}

internal Sound_Handle LoadSound(Asset_Manager *manager, char *path) {
    Sound_Handle result = {};
    if ((manager->asset_count + 1) < manager->max_asset_count) {
        result.index = manager->asset_count + 1;
        Asset *asset = GetAsset(manager, result.index);

        asset->sound = sfSoundBuffer_createFromFile(path);

        if (!asset->sound) { result.index = 0; }
        else {
            asset->type = AssetType_Sound;
            ++manager->asset_count;
        }
    }

    return result;
}

internal Music_Handle LoadMusic(Asset_Manager *manager, char *path) {
    Music_Handle result = {};
    if ((manager->asset_count + 1) < manager->max_asset_count) {
        result.index = manager->asset_count + 1;
        Asset *asset = GetAsset(manager, result.index);

        asset->music = sfMusic_createFromFile(path);

        if (!asset->music) { result.index = 0; }
        else {
            asset->type = AssetType_Music;
            ++manager->asset_count;
        }
    }

    return result;
}

internal Font_Handle LoadFont(Asset_Manager *manager, char *path) {
    Font_Handle result = {};
    if ((manager->asset_count + 1) < manager->max_asset_count) {
        result.index = manager->asset_count + 1;
        Asset *asset = GetAsset(manager, result.index);

        asset->font = sfFont_createFromFile(path);

        if (!asset->font) { result.index = 0; }
        else {
            asset->type = AssetType_Font;
            ++manager->asset_count;
        }
    }

    return result;
}

internal sfTexture *GetImage(Asset_Manager *manager, Image_Handle handle) {
    sfTexture *result = 0;
    if (IsValid(handle)) {
        Asset *asset = GetAsset(manager, handle.index);
        Assert(asset->type == AssetType_Image);

        result = asset->image;
    }

    return result;
}

internal sfSoundBuffer *GetSound(Asset_Manager *manager, Sound_Handle handle) {
    sfSoundBuffer *result = 0;
    if (IsValid(handle)) {
        Asset *asset = GetAsset(manager, handle.index);
        Assert(asset->type == AssetType_Sound);

        result = asset->sound;
    }

    return result;
}

internal sfMusic *GetMusic(Asset_Manager *manager, Music_Handle handle) {
    sfMusic *result = 0;
    if (IsValid(handle)) {
        Asset *asset = GetAsset(manager, handle.index);
        Assert(asset->type == AssetType_Music);

        result = asset->music;
    }

    return result;
}

internal sfFont *GetFont(Asset_Manager *manager, Font_Handle handle) {
    sfFont *result = 0;
    if (IsValid(handle)) {
        Asset *asset = GetAsset(manager, handle.index);
        Assert(asset->type == AssetType_Font);

        result = asset->font;
    }

    return result;
}
