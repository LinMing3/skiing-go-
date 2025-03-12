cmake_minimum_required(VERSION 3.27)
project(MyGame C)

set(CMAKE_C_STANDARD 99)


include_directories(include)
link_directories("./bin/SDL2.dll")
link_directories("./bin/SDL2_image.dll")
link_directories("./bin/SDL2_mixer.dll")
link_directories("./bin/SDL2_ttf.dll")
#link_directories(./SDL2_image.dll)
#link_directories(./SDL2_ttf.dll)
#link_directories(./SDL2_mixer.dll)
include_directories("D:/envs/SDL2/SDL2/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include")
link_directories("D:/envs/SDL2/SDL2/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/lib")

target_link_libraries(MyGame
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)