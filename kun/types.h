#pragma once

#include "engine_config.h"
#include "mathlib.h"
#include <stdio.h>
#include <vector>
#include <array>
#include <map>
#include <stdint.h> // for int32_t, int64_t


//#define	ZeroMemory	memset
//#define ZeroMemory RtlZeroMemory"
//#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
//  C++:  std::fill_n (pcm_arr_, BUFFER_SIZE, 0); or std::fill( array, array + numberOfElements, 0.0 );
#define ZeroMyMemory(Destination,Length) memset((Destination),0,(Length))

