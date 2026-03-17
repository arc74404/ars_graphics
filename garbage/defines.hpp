#pragma once

#define FAILED_RESULT_RETURN(command, success_res) \
    {                                              \
        auto&& res = command;                      \
        if (res != success_res)                    \
        {                                          \
            return res;                            \
        }                                          \
    }

#define FAILED_RESULT_VALUE_RETURN(name_res, command, success_res) \
    auto&& name_res = command;                                     \
    if (name_res.result != success_res)                            \
    {                                                              \
        return name_res.result;                                    \
    }
