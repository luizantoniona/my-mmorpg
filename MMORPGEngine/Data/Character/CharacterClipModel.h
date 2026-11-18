#ifndef CHARACTERCLIPMODEL_H
#define CHARACTERCLIPMODEL_H

namespace Engine {

class CharacterClipModel {
public:
    CharacterClipModel();
    CharacterClipModel( int frameCount, int frameDurationMs );

    int frameCount() const;
    int frameDurationMs() const;

private:
    int _frameCount;
    int _frameDurationMs;
};

} // namespace Engine

#endif // CHARACTERCLIPMODEL_H
