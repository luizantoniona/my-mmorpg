find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
  MMORPGEngineTests
  AnimationModelTest.cpp
  AppearanceFactoryTest.cpp
  CameraTest.cpp
  CharacterDTOTest.cpp
  CharacterIntentEquipDTOTest.cpp
  ChunkDTOTest.cpp
  ChunkModelTest.cpp
  DataFactoryTest.cpp
  EntityCombatModelTest.cpp
  EntityMovementModelTest.cpp
  FloorFactoryTest.cpp
  GroundItemDTOTest.cpp
  ItemCatalogTest.cpp
  ItemCategoryHelperTest.cpp
  ItemFactoryTest.cpp
  ItemTypeCatalogTest.cpp
  ItemTypeFactoryTest.cpp
  JsonHelperTest.cpp
  ManifestFactoryTest.cpp
  CreatureTypeCatalogTest.cpp
  CreatureTypeFactoryTest.cpp
  ObjectCatalogTest.cpp
  ObjectFactoryTest.cpp
  OwnCombatDTOTest.cpp
  RenderWorldTest.cpp
  StreamRenderWorldTest.cpp
  SkillCatalogTest.cpp
  SkillFactoryTest.cpp
  SkillTreeModelTest.cpp
  TileCatalogTest.cpp
  TileFactoryTest.cpp
  WorldFactoryTest.cpp
  WorldModelTest.cpp
)

target_link_libraries(MMORPGEngineTests PRIVATE MMORPG::MMORPGEngine GTest::gtest_main)

gtest_discover_tests(MMORPGEngineTests DISCOVERY_MODE PRE_TEST)
