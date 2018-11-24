//
// Created by hackerl on 10/28/18.
//

#include "SSRLocal.h"
#include <Plugin/LibPluginExport.h>

extern "C" IPlugin * NewPlugin()
{
    return new CSSRLocal;
}