# ===== Common Source Files =====
SET(SAMPLE_COMMON_DIR ${CMAKE_SOURCE_DIR}/vs-mp/common)

SET(SAMPLE_COMMON_SRCS
    ${SAMPLE_COMMON_DIR}/sample_common_sys.c
    ${SAMPLE_COMMON_DIR}/sample_common_vii.c
    ${SAMPLE_COMMON_DIR}/sample_common_vpp.c
    ${SAMPLE_COMMON_DIR}/sample_common_venc.c
    ${SAMPLE_COMMON_DIR}/sample_common_vo.c
    ${SAMPLE_COMMON_DIR}/sample_common_mipitx.c
)

# ===== Local Source Files =====
FILE(GLOB LOCAL_SOURCES "*.c")

# ===== All Source Files =====
SET(SRC_LIST 
    ${LOCAL_SOURCES}
    ${SAMPLE_COMMON_SRCS}
)

# ===== Include Common Directory =====
INCLUDE_DIRECTORIES(${SAMPLE_COMMON_DIR})

# ===== Exe output path =====
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# ===== Create Executable =====
ADD_EXECUTABLE(${APP_NAME} ${SRC_LIST})

# ===== Link Libraries =====
TARGET_LINK_LIBRARIES(
    ${APP_NAME}
    # VS-MP specific libraries
    base
    sys
    gpe
    GAL
    mipirx
    init
    dsp
    vii
    isp
    vpp
    venc
    gdc
    vo
    mipitx
    hdmi
    
    # System libraries
    dl
    m
    pthread
)

MESSAGE(STATUS "Configured ${APP_NAME} with ${CMAKE_BUILD_TYPE} build type")