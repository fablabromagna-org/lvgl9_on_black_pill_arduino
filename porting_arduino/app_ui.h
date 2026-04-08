#pragma once

#include <stdint.h>

namespace porting {
namespace app {

void create_ui();
void update_runtime(uint32_t now_ms);

}  // namespace app
}  // namespace porting
