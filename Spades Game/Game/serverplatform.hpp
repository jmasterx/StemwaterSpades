#ifndef CGE_SERVER_PLATFORM_HPP
#define CGE_SERVER_PLATFORM_HPP
//#define CGE_DEV
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
//used to make mysql play nice with raknet
#ifndef HAVE_INT8_T
#define HAVE_INT8_T
#endif
#include "Game/Utility/Exception.hpp"
#include "Game/Engine/Log.hpp"
namespace cge
{
	typedef long long int64;
}
#endif