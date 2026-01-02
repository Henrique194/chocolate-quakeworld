include("../common/CMakeLists.txt")

set(TARNAME "chocolate-quakeworld-client")

set(SOURCE
    adivtab.h
    anorms.h
    bgmusic.c
    cl_cam.c
    cl_demo.c
    cl_ents.c
    cl_input.c
    cl_main.c
    cl_parse.c
    cl_pred.c
    cl_tent.c
    console.c
    d_edge.c
    d_fill.c
    d_ifacea.h
    d_init.c
    d_local.h
    d_modech.c
    d_part.c
    d_polyse.c
    d_scan.c
    d_sky.c
    d_sprite.c
    d_surf.c
    d_vars.c
    d_zpoint.c
    draw.c
    in_gamepad.c
    in_gamepad.h
    in_keyboard.c
    in_keyboard.h
    in_main.c
    in_mouse.c
    in_mouse.h
    keys.c
    menu.c
    model.c
    r_aclip.c
    r_alias.c
    r_bsp.c
    r_draw.c
    r_edge.c
    r_efrag.c
    r_light.c
    r_local.h
    r_main.c
    r_misc.c
    r_part.c
    r_shared.h
    r_sky.c
    r_sprite.c
    r_surf.c
    r_vars.c
    sbar.c
    screen.c
    skin.c
    snd_codec.c
    snd_codec.h
    snd_dma.c
    snd_flac.c
    snd_flac.h
    snd_mem.c
    snd_mix.c
    snd_mp3.c
    snd_mp3.h
    snd_mp3tag.c
    snd_sdl.c
    snd_vorbis.c
    snd_vorbis.h
    snd_wave.c
    snd_wave.h
    sys.c
    vid_buffers.c
    vid_buffers.h
    vid_main.c
    vid_menu.c
    vid_menu.h
    vid_modes.c
    vid_modes.h
    vid_window.c
    vid_window.h
    view.c
    wad.c
)
list(APPEND SOURCE ${COMMON_SRC})

set(LIBS
    ${SDL2_LIBRARIES}
    $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
)
if(UNIX AND NOT APPLE)
    list(APPEND LIBS
        ${VORBIS_LIBRARY}
        ${VORBISFILE_LIBRARY}
        ${FLAC_LIBRARY}
        ${MAD_LIBRARY}
    )
else()
    list(APPEND LIBS
        Vorbis::vorbisfile
        mad::mad
        FLAC::FLAC
    )
endif()
if(WIN32)
    list(APPEND LIBS winmm ws2_32)
endif()

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
    add_executable(${TARNAME} WIN32 ${SOURCE} ${RC_OUTPUT_FILE})
else()
    add_executable(${TARNAME} ${SOURCE})
endif()

target_include_directories(${TARNAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    "../common"
)

target_link_libraries(${TARNAME} ${LIBS})

if(MSVC)
    set_target_properties(${TARNAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()
