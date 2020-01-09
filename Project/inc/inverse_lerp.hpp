#pragma once

float inverse_lerp(float a, float b, float value)
{
  return (value - a) / (b - a);
}
