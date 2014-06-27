// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOMAIN_RELIABILITY_CONFIG_H_
#define COMPONENTS_DOMAIN_RELIABILITY_CONFIG_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/json/json_value_converter.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "base/values.h"
#include "components/domain_reliability/domain_reliability_export.h"
#include "url/gurl.h"

namespace domain_reliability {

// The configuration that controls which requests are measured and reported,
// with what frequency, and where the beacons are uploaded.
class DOMAIN_RELIABILITY_EXPORT DomainReliabilityConfig {
 public:
  // A particular resource named in the config -- includes a set of URL
  // patterns that the resource will match, along with sample rates for
  // successful and unsuccessful requests.
  class DOMAIN_RELIABILITY_EXPORT Resource {
   public:
    Resource();
    ~Resource();

    // Returns whether |url_string| matches at least one of the |url_patterns|
    // in this Resource.
    bool MatchesUrlString(const std::string& url_string) const;

    // Returns whether a request (that was successful if |success| is true)
    // should be reported (with a full beacon). (The output is random; it
    // compares a random number to |success_sample_rate| or
    // |failure_sample_rate|.)
    bool DecideIfShouldReportRequest(bool success) const;

    // Registers with the JSONValueConverter so it will know how to convert the
    // JSON for a named resource into the struct.
    static void RegisterJSONConverter(
        base::JSONValueConverter<Resource>* converter);

    // Name of the Resource, as will be reported in uploads.
    std::string name;

    // List of URL patterns to assign requests to this Resource.
    ScopedVector<std::string> url_patterns;

    // Sample rates for successful and unsuccessful requests, respectively.
    // 0.0 reports no requests, and 1.0 reports every request.
    double success_sample_rate;
    double failure_sample_rate;

   private:
    DISALLOW_COPY_AND_ASSIGN(Resource);
  };

  // A particular endpoint for report uploads. Includes the URL to upload
  // reports to. May include a verification URL or backoff/load management
  // configuration in the future.
  struct DOMAIN_RELIABILITY_EXPORT Collector {
   public:
    Collector();
    ~Collector();

    // Registers with the JSONValueConverter so it will know how to convert the
    // JSON for a collector into the struct.
    static void RegisterJSONConverter(
        base::JSONValueConverter<Collector>* converter);

    GURL upload_url;

   private:
    DISALLOW_COPY_AND_ASSIGN(Collector);
  };

  DomainReliabilityConfig();
  ~DomainReliabilityConfig();

  // Uses the JSONValueConverter to parse the JSON for a config into a struct.
  static scoped_ptr<const DomainReliabilityConfig> FromJSON(
      const base::StringPiece& json);

  // Finds the index (in resources) of the first Resource that matches a
  // particular URL. Returns -1 if the URL is not matched by any Resources.
  int GetResourceIndexForUrl(const GURL& url) const;

  // Registers with the JSONValueConverter so it will know how to convert the
  // JSON for a config into the struct.
  static void RegisterJSONConverter(
      base::JSONValueConverter<DomainReliabilityConfig>* converter);

  std::string config_version;
  std::string domain;
  ScopedVector<Resource> resources;
  ScopedVector<Collector> collectors;

  // TODO(ttuttle): Add config_valid_util when fetching and expiring configs
  //                is implemented.

 private:
  DISALLOW_COPY_AND_ASSIGN(DomainReliabilityConfig);
};

}  // namespace domain_reliability

#endif  // COMPONENTS_DOMAIN_RELIABILITY_CONFIG_H_
