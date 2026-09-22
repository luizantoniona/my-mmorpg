find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
  MMORPGEngineTests
  AnimationModelTest.cpp
  CameraTest.cpp
  CharacterDTOTest.cpp
  ChunkModelTest.cpp
  FloorFactoryTest.cpp
  ItemCatalogTest.cpp
  ItemFactoryTest.cpp
  ItemTypeCatalogTest.cpp
  ItemTypeFactoryTest.cpp
  JsonHelperTest.cpp
  ManifestFactoryTest.cpp
  ObjectCatalogTest.cpp
  ObjectFactoryTest.cpp
  RenderWorldTest.cpp
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
