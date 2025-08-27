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

    # System libraries
    dl
    m
    pthread
)

MESSAGE(STATUS "Configured ${APP_NAME} with ${CMAKE_BUILD_TYPE} build type")