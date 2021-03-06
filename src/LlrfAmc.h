#ifndef _GEN2LLRF_H_
#define _GEN2LLRF_H_

/**
 *-----------------------------------------------------------------------------
 * Title      : LLRF System Driver
 * ----------------------------------------------------------------------------
 * File       : LlrfAmc.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2020-07-16
 * ----------------------------------------------------------------------------
 * Description:
 * Low level driver for the LLRF systems. It is conformed by a DownConverter
 * and either a Gen1 ot Gen2 Up converter card, depending on which one if
 * present in the YAML description files.
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

#include "UpConverter.h"
#include "DownConverter.h"
#include "Gen1UpConverter.h"
#include "Gen2UpConverter.h"
#include "LlrfAmcLogger.h"

class ILlrfAmc;

typedef boost::shared_ptr<ILlrfAmc> LlrfAmc;

class ILlrfAmc
{
public:
    ILlrfAmc(Path p);

    // Factory method, which returns a smart pointer
    static LlrfAmc create(Path p);

    // Call initialization sequence of both cards
    bool init() const;

    // Check if the down converter card is initialized
    bool isDownConvLocked() const;

    // Check if the up converter card is initialized
    bool isUpConvLocked() const;

    // Check is both cards are initialized
    bool isLocked() const;

    // Get copies of the Down and Up converter objects
    DownConverter   getDownConv() const;
    UpConverter     getUpConv() const;

private:
    static const std::string ModuleName;

    Path            root;

    // Devices
    UpConverter   upConv;
    DownConverter downConv;

    // Logger
    Logger          log;
};

#endif
