// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Strelka - Small Variant Caller
// Copyright (c) 2009-2016 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

/*
 *  \author Morten Kallberg
 */

#pragma once

#include "LogisticAndRuleScoringModels.hh"
#include "starling_shared.hh"
#include "calibration/VariantScoringModelServer.hh"


/// handles site and indel filter labeling OR EVS scoring and filtering
///
/// EVS model is specified in a json container expressing parameters for
/// potentially several model types.
///
/// When EVS cannot be applied, a fallback set of hard filters are applied
/// to the variant.
///
/// Also supports a legacy EVS model expressing both a logistic regression
/// model and a rule based filter, this model is deprecated.
///
struct ScoringModelManager
{
    ScoringModelManager(
        const starling_options& opt,
        const gvcf_deriv_options& gvcfDerivedOptions);

    void
    classify_site(
        const GermlineDiploidSiteCallInfo& si,
        GermlineDiploidSiteSimpleGenotypeInfo& smod) const;

    void
    classify_indel(
        const GermlineDiploidIndelCallInfo& ii,
        GermlineDiploidIndelSimpleGenotypeInfo& call) const;

    void
    classify_indels(
        std::vector<std::unique_ptr<GermlineDiploidIndelCallInfo>>& indels) const;

    // mimics behavior of previous hard filters
    void default_classify_site(
        const GermlineSiteCallInfo& si,
        GermlineVariantSimpleGenotypeInfo& call) const;

    void default_classify_indel(GermlineIndelSimpleGenotypeInfo& call) const;


    int
    get_case_cutoff(const LEGACY_CALIBRATION_MODEL::var_case my_case) const;

    bool
    isNoEVSModel() const
    {
        return (not (_legacyModelPtr || (_snvScoringModelPtr && _indelScoringModelPtr)));
    }

    bool
    isLegacyLogisticEVSModel() const;

private:
    LogisticAndRuleScoringModels& getLegacyModel()
    {
        return *_legacyModelPtr;
    }
    const LogisticAndRuleScoringModels& getLegacyModel() const
    {
        return *_legacyModelPtr;
    }

    bool checkIsVariantUsableInEVSModel(const GermlineDiploidIndelCallInfo& ii) const;

    void set_indel_modifiers(
        const GermlineDiploidIndelCallInfo& ii,
        GermlineDiploidIndelSimpleGenotypeInfo& call) const;

    void
    classify_indel_impl(
        const bool is_model_usable,
        const GermlineDiploidIndelCallInfo& ii,
        GermlineDiploidIndelSimpleGenotypeInfo& call) const;

    bool
    isLegacyModel() const
    {
        return static_cast<bool>(_legacyModelPtr);
    }

    // for setting the vcf header filters
    const gvcf_options& _opt;
    const gvcf_deriv_options& _dopt;
    bool _isReportEVSFeatures;

    std::unique_ptr<LogisticAndRuleScoringModels> _legacyModelPtr;

    std::unique_ptr<VariantScoringModelServer> _snvScoringModelPtr;
    std::unique_ptr<VariantScoringModelServer> _indelScoringModelPtr;

    double _snvEVSThreshold = 0;
    double _indelEVSThreshold = 0;
};

