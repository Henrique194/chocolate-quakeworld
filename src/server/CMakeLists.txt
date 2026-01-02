include("../common/CMakeLists.txt")

set(TARNAME "chocolate-quakeworld-server")

set(SOURCE
    model.c
    pr_cmds.c
    pr_comp.h
    pr_edict.c
    pr_exec.c
    progdefs.h
    progs.h
    qwsvdef.h
    server.h
    sv_ccmds.c
    sv_ents.c
    sv_init.c
    sv_main.c
    sv_move.c
    sv_nchan.c
    sv_phys.c
    sv_send.c
    sv_user.c
    sys.c
    sv_world.c
    sv_world.h
)
list(APPEND SOURCE ${COMMON_SRC})


if(WIN32)
    string(CONCAT RC_VERSION
        "${PROJECT_VERSION_MAJOR}, "
        "${PROJECT_VERSION_MINOR}, "
        "${PROJECT_VERSION_PATCH}, "
        "0"
    )
    set(WIN_DIR "${CMAKE_SOURCE_DIR}/dist/win")
    set(RC_NAME "quake.rc")
    set(RC_INPUT_FILE "${WIN_DIR}/${RC_NAME}.in")
    set(RC_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${RC_NAME}")

    configure_file(${RC_INPUT_FILE} ${RC_OUTPUT_FILE})
    add_executable(${TARNAME} ${SOURCE} ${RC_OUTPUT_FILE})
else()
    add_executable(${TARNAME} ${SOURCE})
endif()

target_compile_definitions(${TARNAME} PRIVATE SERVERONLY)

target_include_directories(${TARNAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    "../common"
)

target_link_libraries(${TARNAME}
    ${SDL2_LIBRARIES}
    $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
)
if(WIN32)
    target_link_libraries(${TARNAME} winmm ws2_32)
endif()

if(MSVC)
    set_target_properties(${TARNAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()
