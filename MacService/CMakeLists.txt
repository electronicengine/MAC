cmake_minimum_required(VERSION 2.8)

project(MacService)


include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    )

link_libraries("pthread")

add_executable(${PROJECT_NAME} 	"main.c"
                                "src/commander_mac.c"
                                "src/commander_phy.c"
                                "src/control_frames.c"
                                "src/mac_command.c"
                                "src/mac_data_sap.c"
                                "src/mac_frames.c"
                                "src/mac_management_sap.c"
                                "src/mac_service.c"
                                "src/management_frames.c"
                                "src/phy_command.c"
                                "src/phy_data_sap.c"
                                "src/phy_management_sap.c"
                                "src/socket.c"
                                "src/mac_message_repo.c"
                                "src/phy_message_repo.c"
                                )
