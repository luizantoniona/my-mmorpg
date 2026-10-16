#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <QImage>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeFactory.h>

namespace {

class CreatureTypeFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_creaturetypefactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Creatures" ] = "Creatures.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeIndex( const std::vector<std::string>& keys ) const {
        Json::Value indexJson;
        indexJson[ "Creatures" ] = Json::Value( Json::arrayValue );
        for ( const std::string& key : keys ) {
            indexJson[ "Creatures" ].append( key );
        }
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Creatures.json" ).string(), indexJson );
    }

    void writeCreature( const std::string& key, const Json::Value& creatureJson ) const {
        std::filesystem::create_directories( _tempDir / "TestMap" / "Creatures" / key );
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Creatures" / key / ( key + ".json" ) ).string(), creatureJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

class InvalidCreatureCase {
public:
    InvalidCreatureCase( std::string name, bool includeName, bool includeVitals, int footprint ) :
        name( std::move( name ) ),
        includeName( includeName ),
        includeVitals( includeVitals ),
        footprint( footprint ) {
    }

    std::string name;
    bool includeName;
    bool includeVitals;
    int footprint;
};

class NumericFieldCase {
public:
    NumericFieldCase( std::string name, std::string field, bool present, int value, uint32_t expected ) :
        name( std::move( name ) ),
        field( std::move( field ) ),
        present( present ),
        value( value ),
        expected( expected ) {
    }

    std::string name;
    std::string field;
    bool present;
    int value;
    uint32_t expected;
};

class CreatureWrongTypeCase {
public:
    CreatureWrongTypeCase( std::string name, std::string parent, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        parent( std::move( parent ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string parent;
    std::string key;
    Json::Value value;
};

class CreatureTypeFactoryWrongTypeTest : public CreatureTypeFactoryTest, public ::testing::WithParamInterface<CreatureWrongTypeCase> {};
class CreatureTypeFactoryInvalidEntryTest : public CreatureTypeFactoryTest, public ::testing::WithParamInterface<InvalidCreatureCase> {};
class CreatureTypeFactoryNumericFieldTest : public CreatureTypeFactoryTest, public ::testing::WithParamInterface<NumericFieldCase> {};

} // namespace

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ParsesAppearanceFromCreatureFolder ) {
    QImage sheet( 8, 4, QImage::Format_ARGB32 );
    sheet.fill( Qt::red );

    writeIndex( { "GOBLIN" } );

    Json::Value creatureType;
    creatureType[ "Name" ] = "Goblin";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 10;
    creatureType[ "Appearance" ][ "FrameSize" ][ 0 ] = 4;
    creatureType[ "Appearance" ][ "FrameSize" ][ 1 ] = 4;
    creatureType[ "Appearance" ][ "Clips" ][ "idle" ][ "File" ] = "idle.png";
    creatureType[ "Appearance" ][ "Clips" ][ "idle" ][ "Rows" ][ "south" ] = 0;
    creatureType[ "Appearance" ][ "Clips" ][ "idle" ][ "Count" ] = 2;
    creatureType[ "Appearance" ][ "Clips" ][ "idle" ][ "FrameDurationMs" ] = 200;
    writeCreature( "GOBLIN", creatureType );

    sheet.save( QString::fromStdString( ( _tempDir / "TestMap" / "Creatures" / "GOBLIN" / "idle.png" ).string() ) );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "GOBLIN" ), nullptr );

    const Engine::AnimationModel& idle = catalog.creatureType( "GOBLIN" )->appearance().animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH );

    EXPECT_TRUE( idle.isAnimated() );
    EXPECT_EQ( idle.frameDurationMs(), 200 );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_WithoutAppearance_HasNullAppearance ) {
    Json::Value creatureType;
    creatureType[ "Name" ] = "Goblin";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 10;

    writeIndex( { "GOBLIN" } );
    writeCreature( "GOBLIN", creatureType );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_TRUE( catalog.creatureType( "GOBLIN" )->appearance().isNull() );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ParsesFields ) {
    Json::Value creatureType;
    creatureType[ "Name" ] = "Goblin";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 10;
    creatureType[ "AggroRadius" ] = 5;

    writeIndex( { "GOBLIN" } );
    writeCreature( "GOBLIN", creatureType );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->type(), "GOBLIN" );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( catalog.creatureType( "GOBLIN" )->vitals().maxHealth(), 10.0 );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->aggroRadius(), 5u );
}

TEST_P( CreatureTypeFactoryInvalidEntryTest, CreateCreatureTypeCatalog_InvalidEntry_IsSkippedAndOthersLoad ) {
    Json::Value invalid;
    if ( GetParam().includeName ) {
        invalid[ "Name" ] = "Goblin";
    }
    if ( GetParam().includeVitals ) {
        invalid[ "Vitals" ][ "MaxHealth" ] = 10;
    }
    if ( GetParam().footprint >= 0 ) {
        invalid[ "Footprint" ] = GetParam().footprint;
    }

    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", invalid );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Entries,
    CreatureTypeFactoryInvalidEntryTest,
    ::testing::Values(
        InvalidCreatureCase( "MissingVitals", true, false, -1 ),
        InvalidCreatureCase( "MissingName", false, true, -1 ),
        InvalidCreatureCase( "ZeroFootprint", true, true, 0 ) ),
    []( const ::testing::TestParamInfo<InvalidCreatureCase>& info ) { return info.param.name; } );

TEST_P( CreatureTypeFactoryWrongTypeTest, CreateCreatureTypeCatalog_WrongFieldType_SkipsEntryAndKeepsOthers ) {
    Json::Value invalid;
    invalid[ "Name" ] = "Goblin";
    invalid[ "Vitals" ][ "MaxHealth" ] = 10;
    if ( GetParam().parent.empty() ) {
        invalid[ GetParam().key ] = GetParam().value;
    } else {
        invalid[ GetParam().parent ][ GetParam().key ] = GetParam().value;
    }

    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", invalid );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    CreatureTypeFactoryWrongTypeTest,
    ::testing::Values(
        CreatureWrongTypeCase( "NameIsNumber", "", "Name", Json::Value( 5 ) ),
        CreatureWrongTypeCase( "VitalsIsNumber", "", "Vitals", Json::Value( 5 ) ),
        CreatureWrongTypeCase( "MaxHealthIsText", "Vitals", "MaxHealth", Json::Value( "ten" ) ),
        CreatureWrongTypeCase( "AggroRadiusIsText", "", "AggroRadius", Json::Value( "far" ) ),
        CreatureWrongTypeCase( "AttackRangeIsNegative", "", "AttackRange", Json::Value( -1 ) ),
        CreatureWrongTypeCase( "FootprintIsText", "", "Footprint", Json::Value( "big" ) ),
        CreatureWrongTypeCase( "AppearanceIsNumber", "", "Appearance", Json::Value( 5 ) ) ),
    []( const ::testing::TestParamInfo<CreatureWrongTypeCase>& info ) { return info.param.name; } );

TEST_P( CreatureTypeFactoryNumericFieldTest, CreateCreatureTypeCatalog_NumericField ) {
    Json::Value creature;
    creature[ "Name" ] = "Creature";
    creature[ "Vitals" ][ "MaxHealth" ] = 3;
    if ( GetParam().present ) {
        creature[ GetParam().field ] = GetParam().value;
    }

    writeIndex( { "CREATURE" } );
    writeCreature( "CREATURE", creature );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    const Engine::CreatureTypeModel* creatureType = catalog.creatureType( "CREATURE" );
    ASSERT_NE( creatureType, nullptr );

    uint32_t actual = 0;
    if ( GetParam().field == "AttackRange" ) {
        actual = creatureType->attackRange();
    } else if ( GetParam().field == "AggroRadius" ) {
        actual = creatureType->aggroRadius();
    } else {
        actual = creatureType->footprint();
    }

    EXPECT_EQ( actual, GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    CreatureTypeFactoryNumericFieldTest,
    ::testing::Values(
        NumericFieldCase( "AttackRange_Missing_DefaultsToMelee", "AttackRange", false, 0, 1 ),
        NumericFieldCase( "AttackRange_Parsed", "AttackRange", true, 4, 4 ),
        NumericFieldCase( "AggroRadius_Missing_DefaultsToPassive", "AggroRadius", false, 0, 0 ),
        NumericFieldCase( "AggroRadius_Parsed", "AggroRadius", true, 5, 5 ),
        NumericFieldCase( "Footprint_Missing_DefaultsToOne", "Footprint", false, 0, 1 ),
        NumericFieldCase( "Footprint_Parsed", "Footprint", true, 3, 3 ) ),
    []( const ::testing::TestParamInfo<NumericFieldCase>& info ) { return info.param.name; } );

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MultipleCreatureTypes_AllAdded ) {
    Json::Value first;
    first[ "Name" ] = "Goblin";
    first[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value second;
    second[ "Name" ] = "Rat";
    second[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", first );
    writeCreature( "RAT", second );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureTypes().size(), 2u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_IndexedKeyWithoutFile_IsSkipped ) {
    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_NE( catalog.creatureType( "RAT" ), nullptr );
}

TEST_F( CreatureTypeFactoryTest, ReadTypeKeys_InvalidAndDuplicatedKeys_AreSkipped ) {
    writeIndex( { "GOBLIN", "goblin", "GOBLIN", "BAD KEY", "RAT" } );

    const QStringList keys = Engine::CreatureTypeFactory::readTypeKeys( configPath() );

    EXPECT_EQ( keys, QStringList( { "GOBLIN", "RAT" } ) );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_ThenCreateCreatureTypeCatalog_RoundTripsFields ) {
    Engine::CreatureTypeCatalog original;
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "GOBLIN" );
    creatureType.setName( "Goblin" );
    creatureType.vitals().setMaxHealth( 10 );
    creatureType.setAggroRadius( 5 );
    creatureType.setAttackRange( 4 );
    original.addCreatureType( creatureType );

    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), original );

    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Creatures" / "GOBLIN" / "GOBLIN.json" ) );

    Engine::CreatureTypeCatalog reloaded;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.creatureType( "GOBLIN" ), nullptr );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( reloaded.creatureType( "GOBLIN" )->vitals().maxHealth(), 10.0 );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->aggroRadius(), 5u );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->attackRange(), 4u );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_PreservesUnknownFieldsInCreatureFile ) {
    Json::Value existing;
    existing[ "Name" ] = "Goblin";
    existing[ "Vitals" ][ "MaxHealth" ] = 10;
    existing[ "Appearance" ][ "FrameSize" ][ 0 ] = 64;

    writeIndex( { "GOBLIN" } );
    writeCreature( "GOBLIN", existing );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );
    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), catalog );

    const Json::Value saved = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Creatures" / "GOBLIN" / "GOBLIN.json" ).string() );

    EXPECT_EQ( saved[ "Appearance" ][ "FrameSize" ][ 0 ].asInt(), 64 );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_RoundTripsFootprint ) {
    Engine::CreatureTypeCatalog original;
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "COW" );
    creatureType.setName( "Cow" );
    creatureType.vitals().setMaxHealth( 30 );
    creatureType.setFootprint( 3 );
    original.addCreatureType( creatureType );

    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), original );

    Engine::CreatureTypeCatalog reloaded;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.creatureType( "COW" ), nullptr );
    EXPECT_EQ( reloaded.creatureType( "COW" )->footprint(), 3u );
}
