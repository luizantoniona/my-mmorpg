find_package(GTest REQUIRED)

add_executable(
  MMORPGEngineTests
  AnimationModelTest.cpp
  CameraTest.cpp
  CharacterDTOTest.cpp
  ChunkModelTest.cpp
  FloorFactoryTest.cpp
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

add_test(NAME MMORPGEngineTests COMMAND MMORPGEngineTests)
