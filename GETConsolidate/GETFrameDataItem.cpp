//
//  GETFrameDataItem.cpp
//  GETConsolidate
//
//  Created by Joshua Bradt on 5/5/14.
//  Copyright (c) 2014 NSCL. All rights reserved.
//

#include "GETFrameDataItem.h"

// --------
// Constructor
// --------

GETFrameDataItem::GETFrameDataItem(uint8_t agetId_in, uint8_t channel_in,
                                   uint16_t timeBucketId_in, uint16_t sample_in)
: agetId(agetId_in),channel(channel_in),timeBucketId(timeBucketId_in),
    sample(sample_in)
{
}

// --------
// Getters
// --------

uint8_t GETFrameDataItem::GetAgetId() const
{
    return this->agetId;
}

uint8_t GETFrameDataItem::GetChannel() const
{
    return this->channel;
}

uint16_t GETFrameDataItem::GetTimeBucketId() const
{
    return this->timeBucketId;
}

uint16_t GETFrameDataItem::GetSample() const
{
    return this->sample;
}