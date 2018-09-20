add_executable(${PROJECT_NAME}
	main.cpp
	dialogueWindow.cpp
	game.cpp
	level.cpp
	objectFactory.cpp
	tileDatabase.cpp
	window.cpp
	room.cpp
	pathfindingGrid.cpp
	pathfinding.cpp
	components/weapon.cpp
	components/type.cpp
	components/traversable.cpp
	components/stats.cpp
	components/render.cpp
	components/rarity.cpp
	components/position.cpp
	components/portable.cpp
	components/koboldAI.cpp
	components/inventory.cpp
	components/health.cpp
	components/equipped.cpp
	components/energy.cpp
	components/damage.cpp
	components/armor.cpp
	components/armorType.cpp
	components/entityType.cpp
	components/door.cpp
)

# include source directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/components)

target_link_libraries(
	${PROJECT_NAME}
	${BLT_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR})