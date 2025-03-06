#pragma once

constexpr unsigned int ticksPerSecond = 60;
constexpr float tickDelta = 1.f / ticksPerSecond;

void MAGE_UpdateTime();