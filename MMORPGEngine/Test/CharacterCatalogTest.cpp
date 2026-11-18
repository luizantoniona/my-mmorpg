#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Character/CharacterCatalog.h>

namespace {

Engine::CharacterAppearanceModel bodyAppearance( const QString& idBody ) {
    Engine::CharacterAppearanceModel appearance;
    appearance.setId( Engine::CharacterAppearanceFeatureEnum::BODY, idBody );

    return appearance;
}

Engine::CharacterAppearanceModel bodyAndUnknownFeatureAppearance() {
    Engine::CharacterAppearanceModel appearance = bodyAppearance( "HERO" );
    appearance.setId( Engine::CharacterAppearanceFeatureEnum::UNKNOWN, "ANYTHING" );

    return appearance;
}

class CharacterCatalogTest : public ::testing::Test {
protected:
    void addBody( const QString& id, const QString& name ) {
        Engine::CharacterBodyModel body;
        body.setId( id );
        body.setName( name );

        _catalog.addBody( body );
    }

    Engine::CharacterCatalog _catalog;
};

class AppearanceValidityCase {
public:
    AppearanceValidityCase( std::string name, Engine::CharacterAppearanceModel appearance, bool expected ) :
        name( std::move( name ) ),
        appearance( std::move( appearance ) ),
        expected( expected ) {
    }

    std::string name;
    Engine::CharacterAppearanceModel appearance;
    bool expected;
};

class AppearanceValidityTest : public CharacterCatalogTest, public ::testing::WithParamInterface<AppearanceValidityCase> {};

TEST_P( AppearanceValidityTest, IsValidAppearance ) {
    addBody( "HERO", "Hero" );
    addBody( "VILLAIN", "Villain" );

    EXPECT_EQ( _catalog.isValidAppearance( GetParam().appearance ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    CharacterCatalog,
    AppearanceValidityTest,
    ::testing::Values(
        AppearanceValidityCase( "KnownBody_IsValid", bodyAppearance( "HERO" ), true ),
        AppearanceValidityCase( "OtherKnownBody_IsValid", bodyAppearance( "VILLAIN" ), true ),
        AppearanceValidityCase( "UnknownBody_IsInvalid", bodyAppearance( "NOBODY" ), false ),
        AppearanceValidityCase( "LowercaseBody_IsInvalid", bodyAppearance( "hero" ), false ),
        AppearanceValidityCase( "EmptySelection_MissesRequiredBody", Engine::CharacterAppearanceModel(), false ),
        AppearanceValidityCase( "UnknownFeatureAlongsideValidBody_IsInvalid", bodyAndUnknownFeatureAppearance(), false ) ),
    []( const ::testing::TestParamInfo<AppearanceValidityCase>& info ) { return info.param.name; } );

TEST_F( CharacterCatalogTest, Options_Body_ListsEveryBodyWithItsName ) {
    addBody( "HERO", "Hero" );
    addBody( "VILLAIN", "Villain" );

    const std::vector<Engine::CharacterOptionModel> options = _catalog.options( Engine::CharacterAppearanceFeatureEnum::BODY, Engine::CharacterAppearanceModel() );

    ASSERT_EQ( options.size(), 2u );
    EXPECT_EQ( options[ 0 ].id(), "HERO" );
    EXPECT_EQ( options[ 0 ].name(), "Hero" );
    EXPECT_EQ( options[ 1 ].id(), "VILLAIN" );
}

TEST_F( CharacterCatalogTest, Options_UnknownFeature_IsEmpty ) {
    addBody( "HERO", "Hero" );

    EXPECT_TRUE( _catalog.options( Engine::CharacterAppearanceFeatureEnum::UNKNOWN, Engine::CharacterAppearanceModel() ).empty() );
}

TEST_F( CharacterCatalogTest, Features_EmptyCatalog_OffersNothing ) {
    EXPECT_TRUE( _catalog.features().empty() );
}

TEST_F( CharacterCatalogTest, Features_WithBodies_OffersTheBody ) {
    addBody( "HERO", "Hero" );

    EXPECT_EQ( _catalog.features(), std::vector<Engine::CharacterAppearanceFeatureEnum>( { Engine::CharacterAppearanceFeatureEnum::BODY } ) );
}

TEST_F( CharacterCatalogTest, IsRequired_Body_IsTrue ) {
    EXPECT_TRUE( _catalog.isRequired( Engine::CharacterAppearanceFeatureEnum::BODY ) );
}

TEST_F( CharacterCatalogTest, IsValidAppearance_EmptyCatalog_RejectsAnyBody ) {
    EXPECT_FALSE( _catalog.isValidAppearance( bodyAppearance( "HERO" ) ) );
}

} // namespace
