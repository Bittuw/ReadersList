#include "stdafx.h"
#include "GlobalObjects.h"

std::shared_ptr<HANDLE> newReader = std::make_shared<HANDLE>(CreateEvent(NULL, TRUE, FALSE, NULL));
std::shared_ptr<HANDLE> _globalExit = std::make_shared<HANDLE>(CreateEvent(NULL, TRUE, FALSE, NULL));