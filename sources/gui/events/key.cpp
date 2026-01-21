#include "key.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unordered_map>

namespace ars_graphics
{
std::optional<Key>
ars_graphics::convertGlfwKey(int key)
{
    static std::unordered_map<int, Key> converter = {
        {GLFW_KEY_A,             Key::A              },
        {GLFW_KEY_B,             Key::B              },
        {GLFW_KEY_C,             Key::C              },
        {GLFW_KEY_D,             Key::D              },
        {GLFW_KEY_E,             Key::E              },
        {GLFW_KEY_F,             Key::F              },
        {GLFW_KEY_G,             Key::G              },
        {GLFW_KEY_H,             Key::H              },
        {GLFW_KEY_I,             Key::I              },
        {GLFW_KEY_J,             Key::J              },
        {GLFW_KEY_K,             Key::K              },
        {GLFW_KEY_L,             Key::L              },
        {GLFW_KEY_M,             Key::M              },
        {GLFW_KEY_N,             Key::N              },
        {GLFW_KEY_O,             Key::O              },
        {GLFW_KEY_P,             Key::P              },
        {GLFW_KEY_Q,             Key::Q              },
        {GLFW_KEY_R,             Key::R              },
        {GLFW_KEY_S,             Key::S              },
        {GLFW_KEY_T,             Key::T              },
        {GLFW_KEY_U,             Key::U              },
        {GLFW_KEY_V,             Key::V              },
        {GLFW_KEY_W,             Key::W              },
        {GLFW_KEY_X,             Key::X              },
        {GLFW_KEY_Y,             Key::Y              },
        {GLFW_KEY_Z,             Key::Z              },

        {GLFW_KEY_0,             Key::NUM_0          },
        {GLFW_KEY_1,             Key::NUM_1          },
        {GLFW_KEY_2,             Key::NUM_2          },
        {GLFW_KEY_3,             Key::NUM_3          },
        {GLFW_KEY_4,             Key::NUM_4          },
        {GLFW_KEY_5,             Key::NUM_5          },
        {GLFW_KEY_6,             Key::NUM_6          },
        {GLFW_KEY_7,             Key::NUM_7          },
        {GLFW_KEY_8,             Key::NUM_8          },
        {GLFW_KEY_9,             Key::NUM_9          },

        {GLFW_KEY_F1,            Key::F1             },
        {GLFW_KEY_F2,            Key::F2             },
        {GLFW_KEY_F3,            Key::F3             },
        {GLFW_KEY_F4,            Key::F4             },
        {GLFW_KEY_F5,            Key::F5             },
        {GLFW_KEY_F6,            Key::F6             },
        {GLFW_KEY_F7,            Key::F7             },
        {GLFW_KEY_F8,            Key::F8             },
        {GLFW_KEY_F9,            Key::F9             },
        {GLFW_KEY_F10,           Key::F10            },
        {GLFW_KEY_F11,           Key::F11            },
        {GLFW_KEY_F12,           Key::F12            },
        {GLFW_KEY_F13,           Key::F13            },
        {GLFW_KEY_F14,           Key::F14            },
        {GLFW_KEY_F15,           Key::F15            },
        {GLFW_KEY_F16,           Key::F16            },
        {GLFW_KEY_F17,           Key::F17            },
        {GLFW_KEY_F18,           Key::F18            },
        {GLFW_KEY_F19,           Key::F19            },
        {GLFW_KEY_F20,           Key::F20            },
        {GLFW_KEY_F21,           Key::F21            },
        {GLFW_KEY_F22,           Key::F22            },
        {GLFW_KEY_F23,           Key::F23            },
        {GLFW_KEY_F24,           Key::F24            },

        {GLFW_KEY_TAB,           Key::TAB            },
        {GLFW_KEY_CAPS_LOCK,     Key::CAPS_LOCK      },
        {GLFW_KEY_LEFT_SHIFT,    Key::LEFT_SHIFT     },
        {GLFW_KEY_RIGHT_SHIFT,   Key::RIGHT_SHIFT    },
        {GLFW_KEY_LEFT_CONTROL,  Key::LEFT_CONTROL   },
        {GLFW_KEY_RIGHT_CONTROL, Key::RIGHT_CONTROL  },
        {GLFW_KEY_LEFT_ALT,      Key::LEFT_ALT       },
        {GLFW_KEY_RIGHT_ALT,     Key::RIGHT_ALT      },
        {GLFW_KEY_LEFT_SUPER,    Key::LEFT_SUPER     },
        {GLFW_KEY_RIGHT_SUPER,   Key::RIGHT_SUPER    },
        {GLFW_KEY_SPACE,         Key::SPACE          },
        {GLFW_KEY_ENTER,         Key::ENTER          },
        {GLFW_KEY_BACKSPACE,     Key::BACKSPACE      },
        {GLFW_KEY_DELETE,        Key::DELETE         },
        {GLFW_KEY_INSERT,        Key::INSERT         },
        {GLFW_KEY_HOME,          Key::HOME           },
        {GLFW_KEY_END,           Key::END            },
        {GLFW_KEY_PAGE_UP,       Key::PAGE_UP        },
        {GLFW_KEY_PAGE_DOWN,     Key::PAGE_DOWN      },

        {GLFW_KEY_UP,            Key::ARROW_UP       },
        {GLFW_KEY_DOWN,          Key::ARROW_DOWN     },
        {GLFW_KEY_LEFT,          Key::ARROW_LEFT     },
        {GLFW_KEY_RIGHT,         Key::ARROW_RIGHT    },

        {GLFW_KEY_GRAVE_ACCENT,  Key::GRAVE_ACCENT   },
        {GLFW_KEY_MINUS,         Key::MINUS          },
        {GLFW_KEY_EQUAL,         Key::EQUAL          },
        {GLFW_KEY_LEFT_BRACKET,  Key::LEFT_BRACKET   },
        {GLFW_KEY_RIGHT_BRACKET, Key::RIGHT_BRACKET  },
        {GLFW_KEY_BACKSLASH,     Key::BACKSLASH      },
        {GLFW_KEY_SEMICOLON,     Key::SEMICOLON      },
        {GLFW_KEY_APOSTROPHE,    Key::APOSTROPHE     },
        {GLFW_KEY_COMMA,         Key::COMMA          },
        {GLFW_KEY_PERIOD,        Key::PERIOD         },
        {GLFW_KEY_SLASH,         Key::SLASH          },

        // Numpad
        {GLFW_KEY_KP_0,          Key::NUMPAD_0       },
        {GLFW_KEY_KP_1,          Key::NUMPAD_1       },
        {GLFW_KEY_KP_2,          Key::NUMPAD_2       },
        {GLFW_KEY_KP_3,          Key::NUMPAD_3       },
        {GLFW_KEY_KP_4,          Key::NUMPAD_4       },
        {GLFW_KEY_KP_5,          Key::NUMPAD_5       },
        {GLFW_KEY_KP_6,          Key::NUMPAD_6       },
        {GLFW_KEY_KP_7,          Key::NUMPAD_7       },
        {GLFW_KEY_KP_8,          Key::NUMPAD_8       },
        {GLFW_KEY_KP_9,          Key::NUMPAD_9       },
        {GLFW_KEY_KP_DECIMAL,    Key::NUMPAD_DECIMAL },
        {GLFW_KEY_KP_DIVIDE,     Key::NUMPAD_DIVIDE  },
        {GLFW_KEY_KP_MULTIPLY,   Key::NUMPAD_MULTIPLY},
        {GLFW_KEY_KP_SUBTRACT,   Key::NUMPAD_SUBTRACT},
        {GLFW_KEY_KP_ADD,        Key::NUMPAD_ADD     },
        {GLFW_KEY_KP_ENTER,      Key::NUMPAD_ENTER   },
        {GLFW_KEY_KP_EQUAL,      Key::NUMPAD_EQUAL   },

        {GLFW_KEY_ESCAPE,        Key::ESCAPE         },
        {GLFW_KEY_PRINT_SCREEN,  Key::PRINT_SCREEN   },
        {GLFW_KEY_SCROLL_LOCK,   Key::SCROLL_LOCK    },
        {GLFW_KEY_PAUSE,         Key::PAUSE_BREAK    },
        {GLFW_KEY_NUM_LOCK,      Key::NUM_LOCK       }
    };

    auto&& it = converter.find(key);

    if (it == converter.end())
    {
        return std::nullopt;
    }
    return it->second;
}

KeyStatus
ars_graphics::convertGlfwStatus(int action)
{
    switch (action)
    {
        case GLFW_PRESS:
            return KeyStatus::PRESS;
        case GLFW_RELEASE:
            return KeyStatus::RELEASE;
        case GLFW_REPEAT:
            return KeyStatus::REPEAT;
        default:
            return KeyStatus::RELEASE;
    }
}
} // namespace ars_graphics
