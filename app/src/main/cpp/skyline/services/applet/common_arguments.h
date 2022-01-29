
#pragma once

namespace skyline::service::applet{
    /**
  * @brief Specifies how the applet should run
  * @url https://switchbrew.org/wiki/Applet_Manager_services#LibraryAppletMode
  */
    enum class LibraryAppletMode : u32 {
        AllForeground = 0x0,
        PartialForeground = 0x1,
        NoUi = 0x2,
        PartialForegroundWithIndirectDisplay = 0x3,
        AllForegroundInitiallyHidden = 0x4,
    };

    /**
   * @brief Common arguments to all LibraryApplets
   * @url https://switchbrew.org/wiki/Applet_Manager_services#CommonArguments
   */
    struct CommonArguments {
        u32 version;
        u32 size;
        u32 api_version;
        u32 theme_color;
        u64 play_startup_sound;
        u64 system_tick;
    };
}