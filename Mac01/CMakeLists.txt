cmake_minimum_required(VERSION 2.8)

project(Mac01)

include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    )

link_libraries("pthread")

add_executable(${PROJECT_NAME} "main.c"
                               "src/mac.c"
                               "src/datatransmitter.c"
                               "src/trxstate.c"
                               "src/cca.c"
                               "src/chainbase.c"
                               "src/unixsocket.c"
                               "src/observer.c"
                               "src/subject.c"
                               "src/datasap.c"
                               "src/managementsap.c"
                               "src/servicetest.c"
                               "src/plmeset.c"
                               )
