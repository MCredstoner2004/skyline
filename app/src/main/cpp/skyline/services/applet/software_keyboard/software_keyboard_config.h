// SPDX-License-Identifier: MPL-2.0
// Copyright © 2022 Skyline Team and Contributors (https://github.com/skyline-emu/)
// Copyright © 2019-2022 Ryujinx Team and Contributors

#include <services/account/IAccountServiceForApplication.h>

namespace skyline::service::applet::swkbd {
    /**
     * @brief Specifies the characters the keyboard should allow you to input
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardMode
     */
    enum class KeyboardMode : u32 {
        Full = 0x0,
        Numeric = 0x1,
        ASCII = 0x2,
        FullLatin = 0x3,
        Alphabet = 0x4,
        SimplifiedChinese = 0x5,
        TraditionalChinese = 0x6,
        Korean = 0x7,
        LanguageSet2 = 0x8,
        LanguageSet2Latin = 0x9,
    };

    /**
     * @brief Specifies the characters that you shouldn't be allowed to input
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardMode
     */
    struct InvalidCharFlag {
        bool space : 1;
        bool atMark : 1;
        bool percent : 1;
        bool slash : 1;
        bool backslash : 1;
        bool numeric : 1;
        bool outsideOfDownloadCode : 1;
        bool outsideOfMiiNickName : 1;
    };

    /**
     * @brief Specifies where the cursor should initially be on the initial string
     * @url https://switchbrew.org/wiki/Software_Keyboard#InitialCursorPos
     */
    enum class InitialCursorPos : u32 {
        First = 0x0,
        Last = 0x1,
    };

    /**
     * @url https://switchbrew.org/wiki/Software_Keyboard#PasswordMode
     */
    enum class PasswordMode : u32 {
        Show = 0x0,
        Hide = 0x1, //!< Hides any inputted text to prevent a password from being leaked
    };

    /**
     * @url https://switchbrew.org/wiki/Software_Keyboard#InputFormMode
     * @note Only applies when 1<= textMaxLength <= 32, otherwise Multiline is used
     */
    enum class InputFormMode : u32 {
        OneLine = 0x0,
        MultiLine = 0x1,
        Separate = 0x2, //!<Used with separateTextPos
    };

    /**
     * @brief Specifies the language of custom dictionary entries
     * @url https://switchbrew.org/wiki/Software_Keyboard#DictionaryLang
     */
    enum class DictionaryLang : u16 {
        Japanese = 0x00,
        AmericanEnglish = 0x01,
        CanadianFrench = 0x02,
        LatinAmericanSpanish = 0x03,
        Reserved1 = 0x04,
        BritishEnglish = 0x05,
        French = 0x06,
        German = 0x07,
        Spanish = 0x08,
        Italian = 0x09,
        Dutch = 0x0A,
        Portuguese = 0x0B,
        Russian = 0x0C,
        Reserved2 = 0x0D,
        SimplifiedChinesePinyin = 0x0E,
        TraditionalChineseCangjie = 0x0F,
        TraditionalChineseSimplifiedCangjie = 0x10,
        TraditionalChineseZhuyin = 0x11,
        Korean = 0x12,
    };

    /**
     * @brief describes a custom dictionary entry
     * @url https://switchbrew.org/wiki/Software_Keyboard#DictionaryInfo
     */
    struct DictionaryInfo {
        u32 offset;
        u16 size;
        DictionaryLang dictionaryLang;
    };

    /**
     * @brief The keyboard config that's common across all versions
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardConfig
     */
    struct CommonKeyboardConfig {
        KeyboardMode keyboardMode;
        std::array<char16_t, 0x9> okText;
        char16_t leftOptionalSymbolKey;
        char16_t rightOptionalSymbolKey;
        bool isPredictionEnabled;
        u8 _pad0_[0x1];
        InvalidCharFlag invalidCharsFlags;
        u8 _pad1_[0x3];
        InitialCursorPos initialCursorPos;
        std::array<char16_t, 0x41> headerText;
        std::array<char16_t, 0x81> subText;
        std::array<char16_t, 0x101> guideText;
        u8 _pad2_[0x2];
        u32 textMaxLength;
        u32 textMinLength;
        PasswordMode passwordMode;
        InputFormMode inputFormMode;
        bool isUseNewLine;
        bool isUseUtf8;
        bool isUseBlurBackground;
        u8 _pad3_[0x1];
        u32 initialStringOffset;
        u32 initialStringLength;
        u32 userDictionaryOffset;
        u32 userDictionaryNum;
        bool isUseTextCheck;
        u8 reserved0[0x3];
    };
    static_assert(sizeof(CommonKeyboardConfig) == 0x3D4);

    /**
     * @brief The keyboard config for the first api version
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardConfig
     */
    struct KeyboardConfigV0 {
        CommonKeyboardConfig commonConfig{};
        u8 _pad0_[0x4]{};
        u64 textCheckCallback{};
    };
    static_assert(sizeof(KeyboardConfigV0) == 0x3E0);

    /**
     * @brief The keyboard config as of api version 0x30007
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardConfig
     */
    struct KeyboardConfigV7 {
        CommonKeyboardConfig commonConfig;
        u8 _pad0_[0x4];
        u64 textCheckCallback;
        std::array<u32, 0x8> separateTextPos;
    };
    static_assert(sizeof(KeyboardConfigV7) == 0x400);

    /**
     * @brief The keyboard config as of api version 0x6000B
     * @url https://switchbrew.org/wiki/Software_Keyboard#KeyboardConfig
     */
    struct KeyboardConfigVB {
        CommonKeyboardConfig commonConfig{};
        std::array<u32, 0x8> separateTextPos{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
        std::array<DictionaryInfo, 0x18> customizedDicInfoList{};
        u8 customizedDicCount{};
        bool isCancelButtonDisabled{};
        u8 reserved1[0xD]{};
        u8 trigger{};
        u8 reserved2[0x4]{};

        KeyboardConfigVB();

        KeyboardConfigVB(const KeyboardConfigV7 &v7config);

        KeyboardConfigVB(const KeyboardConfigV0 &v0config);
    };
    static_assert(sizeof(KeyboardConfigVB) == 0x4C8);
}