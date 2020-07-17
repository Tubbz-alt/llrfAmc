#ifndef _UPCONVERTER_H_
#define _UPCONVERTER_H_

/**
 *-----------------------------------------------------------------------------
 * Title      : UpConverter Card Driver
 * ----------------------------------------------------------------------------
 * File       : UpConverter.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2020-07-17
 * ----------------------------------------------------------------------------
 * Description:
 * Base class for the low level driver for the up converter card.
 * ----------------------------------------------------------------------------
 * This file is part of llrfAmc. It is subject to
 * the license terms in the LICENSE.txt file found in the top-level directory
 * of this distribution and at:
    * https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html.
 * No part of llrfAmc, including this file, may be
 * copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE.txt file.
 * ----------------------------------------------------------------------------
**/

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <yaml-cpp/yaml.h>
#include <cpsw_api_user.h>

#include "CpswTopPaths.h"
#include "JesdRx.h"
#include "JesdTx.h"
#include "Lmk04828.h"
#include "Logger.h"

class IUpConverter;

typedef boost::shared_ptr<IUpConverter>  UpConverter;

class IUpConverter
{
public:
    IUpConverter(Path p, const std::string& moduleName);
    virtual ~IUpConverter() {};

    // This method are specific to Gen1 or Gen2 up converters
    virtual bool init() = 0;
    virtual bool isInited() = 0;

protected:
    Path        root;
    Path        jesdRoot;

    // Devices
    JesdRx      jesdRx;
    JesdTx      jesdTx;
    Lmk04828    lmk;

    // Local commands
    Command     initAmcCardCmd;

    // Logger
    Logger      log;
};

#endif