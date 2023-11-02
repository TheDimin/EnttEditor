#pragma once
namespace EnttEditor
{
#ifndef __clang__
#define STATICTEMPLATE static
#else
#define STATICTEMPLATE inline
#endif
}  // namespace EnttEditor