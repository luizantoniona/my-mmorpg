#ifndef TEXTUREIMPORTHELPER_H
#define TEXTUREIMPORTHELPER_H

#include <QString>

class TextureImportHelper {
public:
    static QString toLocalFilePath( const QString& path );

    static bool isAnimatedSource( const QString& sourcePath );
    static QString destinationExtension( const QString& sourcePath );

    static bool importTexture( const QString& sourcePath, const QString& mapPath, const QString& textureFile, const QString& extension );
    static void removeTextureFiles( const QString& mapPath, const QString& textureFile );
};

#endif // TEXTUREIMPORTHELPER_H
