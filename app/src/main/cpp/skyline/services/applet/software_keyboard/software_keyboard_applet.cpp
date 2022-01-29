// SPDX-License-Identifier: MPL-2.0
// Copyright © 2022 Skyline Team and Contributors (https://github.com/skyline-emu/)

#include <codecvt>
#include <services/am/storage/VectorIStorage.h>
#include <common/settings.h>
#include "software_keyboard_applet.h"
#include <jvm.h>

class Utf8Utf16Converter : public std::codecvt<char16_t, char8_t, std::mbstate_t> {
  public:
    ~Utf8Utf16Converter() override = default;
};

namespace skyline::service::applet::swkbd {

    SoftwareKeyboardApplet::SoftwareKeyboardApplet(const DeviceState &state, service::ServiceManager &manager, std::shared_ptr<kernel::type::KEvent> onAppletStateChanged, std::shared_ptr<kernel::type::KEvent> onNormalDataPushFromApplet, std::shared_ptr<kernel::type::KEvent> onInteractiveDataPushFromApplet, service::applet::LibraryAppletMode appletMode)
        : IApplet(state, manager, std::move(onAppletStateChanged), std::move(onNormalDataPushFromApplet), std::move(onInteractiveDataPushFromApplet), appletMode) {
    }

    std::string SoftwareKeyboardApplet::GetUserInput() {
        return state.jvm->RequestTextInput();
    }

    void WriteUTF16toSpan(std::string_view text, span<u8> output_span) {
        auto charData{output_span.cast<char16_t>()};
        Utf8Utf16Converter converter;
        Utf8Utf16Converter::state_type codecvtState;
        const char8_t *next;
        char16_t *nextu16;
        converter.in(codecvtState, reinterpret_cast<const char8_t *>(text.begin()), reinterpret_cast<const char8_t *>(text.end()), next, charData.begin().base(), charData.end().base(), nextu16);
        *nextu16 = u'\0';
    }

    void SoftwareKeyboardApplet::PushUTF16TextForCheck(std::string_view text){
        std::vector<u8> outputData(0x7DC);
        span dataSpan{outputData};
        dataSpan.as<u64>() = outputData.size();
        WriteUTF16toSpan(text, dataSpan.subspan(sizeof(u64)));
        PushInteractiveDataAndSignal(std::make_shared<service::am::VectorIStorage>(state, manager, std::move(outputData)));
    }

    void SoftwareKeyboardApplet::PushUTF16OutputText(std::string_view text){
        std::vector<u8> outputData(0x7D8);
        span dataSpan{outputData};
        dataSpan.as<CloseResult>() = CloseResult::Enter;
        WriteUTF16toSpan(text,dataSpan.subspan(sizeof(CloseResult)));
        PushNormalDataAndSignal(std::make_shared<service::am::VectorIStorage>(state, manager, std::move(outputData)));
    }


    Result SoftwareKeyboardApplet::Start() {
        common_args = normalInputData.front()->GetSpan().as<CommonArguments>();
        normalInputData.pop();

        auto configSpan = normalInputData.front()->GetSpan();
        config = [&] {
            if (common_args.api_version < 0x30007)
                return KeyboardConfigVB{configSpan.as<KeyboardConfigV0>()};
            else if (common_args.api_version < 0x6000B)
                return KeyboardConfigVB{configSpan.as<KeyboardConfigV7>()};
            else
                return configSpan.as<KeyboardConfigVB>();
        }();
        normalInputData.pop();

        workBufferStorage = normalInputData.front();
        normalInputData.pop();

        auto user_input = GetUserInput();
        if(config.commonConfig.isUseTextCheck) {
            PushUTF16TextForCheck(user_input);
            verification_pending = true;
            current_text = user_input;
        } else {
            PushUTF16OutputText(user_input);
        }
        return {};
    }

    Result SoftwareKeyboardApplet::GetResult() {
        return {};
    }

    void SoftwareKeyboardApplet::PushNormalDataToApplet(std::shared_ptr<service::am::IStorage> data) {
        normalInputData.emplace(data);
    }

    void SoftwareKeyboardApplet::PushInteractiveDataToApplet(std::shared_ptr<service::am::IStorage> data) {
        if (data->GetSpan().as<TextCheckResult>() == TextCheckResult::Success) {
            verification_pending = false;
            PushUTF16OutputText(current_text);
            onAppletStateChanged->Signal();
        }
    }

}