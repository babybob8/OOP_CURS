#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config
{
    constexpr std::string_view DefaultLogPath = "log.txt";
    constexpr std::string_view LogPath = DefaultLogPath; /* я подумал типо можно будет  менять   LogPath
                                                            и типо изначально он дефолт, а потом подумал
                                                            что я не оч понимаю   как   это    constexpr
                                                            сделать. ну лан пусть так пока останеться */

    constexpr bool DebugEnabled =
    #ifdef DEBUG_BUILD
        true;
    #else
        false;
    #endif
}

#endif // CONFIG_H
