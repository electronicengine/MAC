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
                                "src/chainbase.c"
                                "src/unixsocket.c"
                                "src/observer.c"
                                "src/subject.c"
                                "src/datasap.c"
                                "src/managementsap.c"
                                "src/servicetest.c")
