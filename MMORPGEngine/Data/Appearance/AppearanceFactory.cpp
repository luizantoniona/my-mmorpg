#include "AppearanceFactory.h"

#include <QDebug>
#include <QHash>
#include <QImage>
#include <QRect>
#include <QSize>

#include <MMORPGEngine/Data/Appearance/AppearanceDirectionHelper.h>
#include <MMORPGEngine/Data/Appearance/AppearanceStateHelper.h>

namespace Engine {

namespace {

bool readSize( const Json::Value& json, QSize& size ) {
    if ( !json.isArray() || json.size() != 2 || !json[ 0 ].isInt() || !json[ 1 ].isInt() ) {
        return false;
    }

    size = QSize( json[ 0 ].asInt(), json[ 1 ].asInt() );

    return size.width() > 0 && size.height() > 0;
}

bool readPositiveInt( const Json::Value& json, const char* key, int& value ) {
    if ( !json.isMember( key ) || !json[ key ].isInt() || json[ key ].asInt() <= 0 ) {
        return false;
    }

    value = json[ key ].asInt();

    return true;
}

QImage loadSheet( QHash<QString, QImage>& sheets, const QString& path ) {
    auto it = sheets.find( path );

    if ( it == sheets.end() ) {
        const QImage image( path );
        it = sheets.insert( path, image.isNull() ? image : image.convertToFormat( QImage::Format_ARGB32_Premultiplied ) );
    }

    return it.value();
}

QList<QImage> cutFrames( const QImage& sheet, const QSize& frameSize, const QSize& stride, int row, int from, int count ) {
    QList<QImage> frames;

    for ( int i = 0; i < count; ++i ) {
        const QRect rect( QPoint( ( from + i ) * stride.width(), row * stride.height() ), frameSize );

        if ( !sheet.rect().intersects( rect ) ) {
            return {};
        }

        frames.append( sheet.copy( rect ) );
    }

    return frames;
}

void createClip( const Json::Value& appearanceJson, const Json::Value& clipJson, AppearanceStateEnum state, const QString& folderPath, QHash<QString, QImage>& sheets, AppearanceModel& appearance ) {
    const QString stateName = QString::fromStdString( AppearanceStateHelper::toString( state ) );

    if ( !clipJson.isMember( "File" ) || !clipJson[ "File" ].isString() || !clipJson.isMember( "Rows" ) || !clipJson[ "Rows" ].isObject() ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Clip needs File and Rows, skipping" << stateName;
        return;
    }

    QSize frameSize;
    if ( !readSize( clipJson.isMember( "FrameSize" ) ? clipJson[ "FrameSize" ] : appearanceJson[ "FrameSize" ], frameSize ) ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Clip without valid FrameSize, skipping" << stateName;
        return;
    }

    QSize stride = frameSize;
    if ( clipJson.isMember( "Stride" ) && !readSize( clipJson[ "Stride" ], stride ) ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Clip with invalid Stride, skipping" << stateName;
        return;
    }

    int count = 0;
    int frameDurationMs = 0;
    if ( !readPositiveInt( clipJson, "Count", count ) || !readPositiveInt( clipJson, "FrameDurationMs", frameDurationMs ) ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Clip needs positive Count and FrameDurationMs, skipping" << stateName;
        return;
    }

    const Json::Value& fromJson = clipJson[ "From" ];
    if ( !fromJson.isNull() && ( !fromJson.isInt() || fromJson.asInt() < 0 ) ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Clip with invalid From, skipping" << stateName;
        return;
    }

    const int from = fromJson.isNull() ? 0 : fromJson.asInt();

    const QString file = QString::fromStdString( clipJson[ "File" ].asString() );
    const QImage sheet = loadSheet( sheets, folderPath + file );
    if ( sheet.isNull() ) {
        qWarning() << "AppearanceFactory::createClip"
                   << "Sheet not found, skipping" << folderPath + file;
        return;
    }

    for ( const std::string& directionName : clipJson[ "Rows" ].getMemberNames() ) {
        const AppearanceDirectionEnum direction = AppearanceDirectionHelper::fromString( directionName );
        const Json::Value& rowJson = clipJson[ "Rows" ][ directionName ];

        if ( direction == AppearanceDirectionEnum::UNKNOWN || !rowJson.isInt() || rowJson.asInt() < 0 ) {
            qWarning() << "AppearanceFactory::createClip"
                       << "Invalid direction row, skipping" << QString::fromStdString( directionName );
            continue;
        }

        const QList<QImage> frames = cutFrames( sheet, frameSize, stride, rowJson.asInt(), from, count );
        if ( frames.isEmpty() ) {
            qWarning() << "AppearanceFactory::createClip"
                       << "Frames outside the sheet, skipping" << stateName + "/" + QString::fromStdString( directionName );
            continue;
        }

        appearance.setAnimation( state, direction, AnimationModel( frames, frameDurationMs ) );
    }
}

} // namespace

AppearanceModel AppearanceFactory::createAppearance( const Json::Value& json, const QString& folderPath ) {
    AppearanceModel appearance;

    if ( !json.isMember( "Clips" ) || !json[ "Clips" ].isObject() ) {
        qWarning() << "AppearanceFactory::createAppearance"
                   << "Appearance without Clips" << folderPath;
        return appearance;
    }

    QHash<QString, QImage> sheets;

    for ( const std::string& stateName : json[ "Clips" ].getMemberNames() ) {
        const AppearanceStateEnum state = AppearanceStateHelper::fromString( stateName );

        if ( state == AppearanceStateEnum::UNKNOWN ) {
            qWarning() << "AppearanceFactory::createAppearance"
                       << "Unknown state, skipping" << QString::fromStdString( stateName );
            continue;
        }

        createClip( json, json[ "Clips" ][ stateName ], state, folderPath, sheets, appearance );
    }

    return appearance;
}

} // namespace Engine
