/**
 *-----------------------------------------------------------------------------
 * Title      : Gen2 LLRF System Driver
 * ----------------------------------------------------------------------------
 * File       : Gen2Llrf.cpp
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2020-07-16
 * ----------------------------------------------------------------------------
 * Description:
 * Low level driver for the LLRF system conformed by the Gen2UpConverter and
 * DownConverter cards.
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

#include <unistd.h>
#include "Gen2Llrf.h"

const std::string IGen2Llrf::ModuleName = "Gen2Llrf";

Gen2Llrf IGen2Llrf::create(Path p)
{
    if(!p)
        throw std::runtime_error(ModuleName + " : The root Path is empty");

    return boost::make_shared<IGen2Llrf>(p);
}

IGen2Llrf::IGen2Llrf(Path p)
:
    root     ( p ),
    log      ( ILogger::create(ModuleName.c_str()) )
{
    // Check if the downconverter module exists
    std::string dcmn ( IDownConverter::getModuleName() );
    log->log(LoggerLevel::Debug, "Verifying if the down converter module " + dcmn + " exists under " + CpswTopPaths::AppCore);
    Path dcp { tryFindPath(root, CpswTopPaths::AppCore + dcmn) };

    // If the down converter module is not found, abort
    if (!dcp)
        throw std::runtime_error("DownConverter module '" + dcmn + "' not found!");

    log->log(LoggerLevel::Debug, "It exist. Creating a DownConverter object.");
    downConv = IDownConverter::create(root);

    // Check if the gen1 up converter exists
    std::string g1ucmn ( "AmcMrLlrfUpConvert" );
    log->log(LoggerLevel::Debug, "Verifying if the gen1 up converter module " + g1ucmn + " exists under " + CpswTopPaths::AppCore);
    Path g1ucp { tryFindPath(root, CpswTopPaths::AppCore + g1ucmn) };

    // If the gen1 up converter module is not found, try to find the gen2 up converter module
    if (!g1ucp)
    {
        log->log(LoggerLevel::Debug, "Gen1 up converter module '" + g1ucmn + "' not found!");

        // Check if the gen2 up converter exists
        std::string g2ucmn ( IGen2UpConverter::getModuleName() );
        log->log(LoggerLevel::Debug, "Verifying if the gen2 up converter module " + g2ucmn + " exists under " + CpswTopPaths::AppCore);
        Path g2ucp { tryFindPath(root, CpswTopPaths::AppCore + g2ucmn) };

        // If neither gen1 nor gen2 up converter modules are found, abort
        if (!dcp)
            throw std::runtime_error("UpConverter modules '" + g1ucmn + "' or '" + g2ucmn + "'not found!");

        log->log(LoggerLevel::Debug, "It exist. Creating a Gen2UpConverter object.");
        upConv = IGen2UpConverter::create(root);
    }
    else
    {
        log->log(LoggerLevel::Debug, "It exist. Creating a Gen1UpConveter object.");
        //upConv = IGen1UpConverter::create(root);
    }

    log->log(LoggerLevel::Debug, "Object created");
}

bool IGen2Llrf::init() const
{
    log->log(LoggerLevel::Debug, "Initializing...");

    // Initialization sequence
    bool success = downConv->init();
    success &= upConv->init();

    if ( success )
        log->log(LoggerLevel::Debug, "Initialization succeed!");
    else
         log->log(LoggerLevel::Error, "Initialization failed!");

    return success;
}

bool IGen2Llrf::isInited() const
{
    return ( downConv->isInited() & upConv->isInited() );
}

bool IGen2Llrf::isDownConvInited() const
{
    return downConv->isInited();
}

bool IGen2Llrf::isUpConvInited() const
{
    return upConv->isInited();
}

DownConverter IGen2Llrf::getDownConv() const
{
   return downConv;
}

UpConverter IGen2Llrf::getUpConv() const
{
    return upConv;
}
