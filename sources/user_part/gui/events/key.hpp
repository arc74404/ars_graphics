#pragma once

#include <optional>

namespace user_part
{
enum class Key
{
    // letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Digits
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,

    // functional
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    // reduct
    TAB,
    CAPS_LOCK,
    SHIFT,
    CONTROL,
    ALT,
    SPACE,
    ENTER,
    BACKSPACE,
    DELETE,
    INSERT,
    HOME,
    END,
    PAGE_UP,
    PAGE_DOWN,

    // arrows
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,

    // symbols
    GRAVE_ACCENT,  // `
    MINUS,         // -
    EQUAL,         // =
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    BACKSLASH,     //
    SEMICOLON,     // ;
    APOSTROPHE,    // '
    COMMA,         // ,
    PERIOD,        // .
    SLASH,         // /

    // Numpad (цифровая клавиатура)
    NUMPAD_0,
    NUMPAD_1,
    NUMPAD_2,
    NUMPAD_3,
    NUMPAD_4,
    NUMPAD_5,
    NUMPAD_6,
    NUMPAD_7,
    NUMPAD_8,
    NUMPAD_9,
    NUMPAD_DECIMAL,
    NUMPAD_DIVIDE,
    NUMPAD_MULTIPLY,
    NUMPAD_SUBTRACT,
    NUMPAD_ADD,
    NUMPAD_ENTER,
    NUMPAD_EQUAL,
    NUMPAD_COMMA,
    NUMPAD_CLEAR,

    VOLUME_UP,
    VOLUME_DOWN,
    VOLUME_MUTE,
    MEDIA_PLAY_PAUSE,
    MEDIA_NEXT_TRACK,
    MEDIA_PREVIOUS_TRACK,
    MEDIA_STOP,
    MEDIA_EJECT,

    POWER,
    SLEEP,
    WAKE,

    PRINT_SCREEN,
    SCROLL_LOCK,
    PAUSE_BREAK,

    LEFT_SHIFT,
    RIGHT_SHIFT,
    LEFT_CONTROL,
    RIGHT_CONTROL,
    LEFT_ALT,
    RIGHT_ALT,
    LEFT_SUPER,
    RIGHT_SUPER,

    ESCAPE,
    CLEAR,
    HELP,

    UNKNOWN,

    NUM_LOCK,
    COMPOSE,
    KANA,
    KANJI,

    NONE
};

enum class KeyStatus
{
    PRESS,
    RELEASE,
    REPEAT
};

std::optional<Key>
convertGlfwKey(int key);

KeyStatus
convertGlfwStatus(int action);

} // namespace user_part
