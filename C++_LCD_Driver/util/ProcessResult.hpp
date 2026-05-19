#pragma once

struct ProcessResult
{
    int  exit_code       = 0;     // valid only if exited_normally == true (0~255)
    bool exited_normally = true;  // false on signal termination etc.
};
