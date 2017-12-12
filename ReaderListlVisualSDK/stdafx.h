#include <Qt>
#include <QClipboard>
#include <QtWidgets>
#include <QWidget>
#include <memory>
#include <set>
#include <stdio.h>
#include <tchar.h>
#include "ZReader.h"
#include "ZPort.h"

#if !defined(ZREADER_LINKONREQUEST)
#pragma comment(lib, "ZReader.lib")
#elif !defined(ZPORT_LINKONREQUEST)
#pragma comment(lib, "ZReader.lib")
#endif // !ZREADER_LINKONREQUEST