cmake_minimum_required(VERSION 2.8)


project(PhySim)

link_libraries("pthread")

include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    )

add_executable(${PROJECT_NAME} "main.c"
                                "src/phy.c"
                                "src/datatransmitter.c"
                                "src/trxstate.c"
                                "src/cca.c"
                                "src/setphy.c"
                                "src/macsocket.c"
                                "src/observer.c"
                                "src/subject.c"
                                "src/mac_message_repo.c"
                                "src/phy_message_repo.c"
                                "src/phy_pib_attribute.c")
