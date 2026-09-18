#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include <QCryptographicHash>
#include <QFile>

#include <MMORPGEngine/Data/Manifest/ManifestFactory.h>

namespace {

class ManifestFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_manifestfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" / "sub" );

        writeFile( _tempDir / "Config.json", R"({"ActiveFolder":"TestMap"})" );
        writeFile( _tempDir / "TestMap" / "Map.json", R"({"Name":"Test"})" );
        writeFile( _tempDir / "TestMap" / "sub" / "file.txt", "nested-content" );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    static void writeFile( const std::filesystem::path& path, const std::string& content ) {
        std::ofstream file( path );
        file << content;
    }

    const Engine::ManifestDataModel* findByPath( const Engine::ManifestModel& manifest, const std::string& path ) const {
        for ( const Engine::ManifestDataModel& data : manifest.data() ) {
            if ( data.path() == path ) {
                return &data;
            }
        }
        return nullptr;
    }

    std::string configPath() const {
        return _tempDir.string() + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( ManifestFactoryTest, CreateManifest_IncludesConfigAndAllMapFiles ) {
    const Engine::ManifestModel manifest = Engine::ManifestFactory::createManifest( configPath() );

    EXPECT_NE( findByPath( manifest, "Config.json" ), nullptr );
    EXPECT_NE( findByPath( manifest, "TestMap/Map.json" ), nullptr );
    EXPECT_NE( findByPath( manifest, "TestMap/sub/file.txt" ), nullptr );
}

TEST_F( ManifestFactoryTest, CreateManifest_HashMatchesIndependentMd5 ) {
    const Engine::ManifestModel manifest = Engine::ManifestFactory::createManifest( configPath() );

    const auto* mapJsonData = findByPath( manifest, "TestMap/Map.json" );
    ASSERT_NE( mapJsonData, nullptr );

    QFile file( QString::fromStdString( ( _tempDir / "TestMap" / "Map.json" ).string() ) );
    ASSERT_TRUE( file.open( QIODevice::ReadOnly ) );
    const QByteArray content = file.readAll();
    const std::string expectedHash = QCryptographicHash::hash( content, QCryptographicHash::Md5 ).toHex().toStdString();

    EXPECT_EQ( mapJsonData->hash(), expectedHash );
}

TEST( ManifestFactoryHashFileTest, HashFile_NonexistentFile_ReturnsEmptyString ) {
    EXPECT_EQ( Engine::ManifestFactory::hashFile( "definitely_missing_file_xyz.json" ), "" );
}
