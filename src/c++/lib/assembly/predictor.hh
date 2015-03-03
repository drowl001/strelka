// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Starka
// Copyright (c) 2009-2014 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/sequencing/licenses/>
//
/*
 * predictor.hh
 *
 *  Created on: Aug 10, 2013
 *  Author: Morten Kallberg
 */

#pragma once

#include "gvcf_locus_info.hh"
#include "starling_common/starling_base_shared.hh"
#include "starling_common/starling_read_buffer.hh"
#include "blt_util/RegionTracker.hh"

//#define DEBUG_predictor


#ifdef DEBUG_predictor
#include "blt_util/log.hh"
#endif

struct predictor
{
    predictor(
       const std::string& regions)
        : regions_file(regions)
    {
        //init regions file bed_streamer, TODO move this code to Starling_run
//        std::unique_ptr<bed_streamer> assemble_regions;
//        if (! opt.gvcf.nocompress_region_bedfile.empty())
//        {
//            nocompress_regions.reset(new bed_streamer(opt.gvcf.nocompress_region_bedfile.c_str(),
//                                                      bam_region.c_str()));
//            sdata.register_assemble_regions(*assemble_regions);
//        }
        //add region from bed to region-tracker
//        while(bedtrack){
//            known_pos_range2 range(239692924,239695935);
//            this->rt.addRegion(range);
//        }

        //add in dummy dev regions
        known_pos_range2 range(239692924,239695935);
        this->rt.addRegion(range);

    }

    /// given an assembler with a region buffered, predict if it should it be assembled
    bool do_assemble();

    /// given an with a region buffered, predict if it should it be extended
    bool keep_extending();


private:
    int assembleCount, assembleContigLength;          // count of regions to assemble, cummulative length of assembled regions
    std::string regions_file;
    RegionTracker rt;
};
