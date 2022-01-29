// SPDX-License-Identifier: MPL-2.0
// Copyright © 2022 Skyline Team and Contributors (https://github.com/skyline-emu/)

#include "applet_creator.h"
#include "services/applet/software_keyboard/software_keyboard_applet.h"

namespace skyline::service::applet {
    /**
     * @brief Switch for creating the appropiate class of Applet
     * TODO add a default stub instead of returing nullptr
     */
    std::shared_ptr<service::am::IApplet> CreateApplet(const DeviceState &state, service::ServiceManager &manager, service::applet::AppletId appletId, const std::shared_ptr<kernel::type::KEvent> &onAppletStateChanged, const std::shared_ptr<kernel::type::KEvent> &onNormalDataPushFromApplet, const std::shared_ptr<kernel::type::KEvent> &onInteractiveDataPushFromApplet, service::applet::LibraryAppletMode appletMode) {
        switch (appletId) {
            case AppletId::LibraryAppletSwkbd:
                return std::make_shared<swkbd::SoftwareKeyboardApplet>(state, manager, onAppletStateChanged, onNormalDataPushFromApplet, onInteractiveDataPushFromApplet, appletMode);
            default:
                return {};
        }
    }
}