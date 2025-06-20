#pragma once

namespace SDL
{
void Init();
void Shutdown();
void AbortIfFailed(bool success);
}