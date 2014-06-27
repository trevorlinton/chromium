// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_RTP_RECEIVER_RTP_PARSER_RTP_PARSER_H_
#define MEDIA_CAST_RTP_RECEIVER_RTP_PARSER_RTP_PARSER_H_

#include "media/cast/rtp_receiver/rtp_receiver_defines.h"
#include "media/cast/transport/cast_transport_defines.h"

namespace media {
namespace cast {

struct RtpParserConfig {
  RtpParserConfig() {
    ssrc = 0;
    payload_type = 0;
    audio_channels = 0;
  }

  uint32 ssrc;
  int payload_type;
  transport::AudioCodec audio_codec;
  transport::VideoCodec video_codec;
  int audio_channels;
};

class RtpParser {
 public:
  RtpParser(const RtpParserConfig parser_config);

  virtual ~RtpParser();

  bool ParsePacket(const uint8* packet,
                   size_t length,
                   RtpCastHeader* rtp_header);
 protected:
  virtual void OnReceivedPayloadData(const uint8* payload_data,
                                     size_t payload_size,
                                     const RtpCastHeader& rtp_header) = 0;
 private:
  bool ParseCommon(const uint8* packet,
                   size_t length,
                   RtpCastHeader* rtp_header);

  bool ParseCast(const uint8* packet, size_t length, RtpCastHeader* rtp_header);

  RtpParserConfig parser_config_;
  transport::FrameIdWrapHelper frame_id_wrap_helper_;
  transport::FrameIdWrapHelper reference_frame_id_wrap_helper_;

  DISALLOW_COPY_AND_ASSIGN(RtpParser);
};

}  // namespace cast
}  // namespace media

#endif  // MEDIA_CAST_RTP_RECEIVER_RTP_PARSER_RTP_PARSER_H_
