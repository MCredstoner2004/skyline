// SPDX-License-Identifier: MPL-2.0
// Copyright © 2022 Skyline Team and Contributors (https://github.com/skyline-emu/)

#pragma once

#include <services/am/storage/IStorage.h>
#include <kernel/types/KEvent.h>
#include <services/applet/common_arguments.h>
#include <queue>

namespace skyline::service::am {
    /**
     * @brief The base class all Applets have to inherit from
     */
    class IApplet : public BaseService {
      private:
        std::shared_ptr<kernel::type::KEvent> onNormalDataPushFromApplet;
        std::shared_ptr<kernel::type::KEvent> onInteractiveDataPushFromApplet;

      protected:
        std::shared_ptr<kernel::type::KEvent> onAppletStateChanged;
        std::queue<std::shared_ptr<IStorage>> normalOutputData;
        std::queue<std::shared_ptr<IStorage>> interactiveOutputData;

        void PushNormalDataAndSignal(const std::shared_ptr<IStorage> &data);

        void PushInteractiveDataAndSignal(const std::shared_ptr<IStorage> &data);

      public:
        IApplet(const DeviceState &state, ServiceManager &manager, std::shared_ptr<kernel::type::KEvent> onAppletStateChanged, std::shared_ptr<kernel::type::KEvent> onNormalDataPushFromApplet, std::shared_ptr<kernel::type::KEvent> onInteractiveDataPushFromApplet, applet::LibraryAppletMode appletMode);

        virtual Result Start() = 0;

        virtual Result GetResult() = 0;

        virtual ~IApplet();

        virtual void PushNormalDataToApplet(std::shared_ptr<IStorage> data) = 0;

        virtual void PushInteractiveDataToApplet(std::shared_ptr<IStorage> data) = 0;

        std::shared_ptr<IStorage> PopNormalAndClear();

        std::shared_ptr<IStorage> PopInteractiveAndClear();
    };
}
