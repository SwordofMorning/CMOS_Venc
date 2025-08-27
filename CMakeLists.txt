PROJECT(VS839_VENC)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# ===== Step 1 : Set Cross Compiler Path =====

SET(_CROSS_COMPILE_ROOT_PATH_ "/hdd/System/VS819L/vs-linux/x86-arm/gcc-linaro-7.5.0-aarch64-linux-gnu")
SET(_CROSS_COMPILE_PATH_ "${_CROSS_COMPILE_ROOT_PATH_}/bin")

SET(CMAKE_C_COMPILER ${_CROSS_COMPILE_PATH_}/aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER ${_CROSS_COMPILE_PATH_}/aarch64-linux-gnu-g++)

# ===== Step 9 : Set Application Name =====
SET(APP_NAME "spitransfer_GNU")

# ===== Step 10 : Add Subdirectory =====
ADD_SUBDIRECTORY(src bin)

# ===== Step 11: Generate Version Info =====
# Branch
EXECUTE_PROCESS(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Last commit ID
EXECUTE_PROCESS(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_ID
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Compile time
STRING(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M:%S")

MESSAGE(STATUS "Building ${APP_NAME} - Branch: ${GIT_BRANCH}, Commit: ${GIT_COMMIT_ID}")