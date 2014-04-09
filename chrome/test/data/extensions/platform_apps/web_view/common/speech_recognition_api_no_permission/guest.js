// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// A guest that run Web Speech API.
// Note that the embedder has to initiate a postMessage first so that
// the guest has a reference to the embedder's window.
window.console.log('guest.js load begin');

var LOG = function(msg) {
  window.console.log(msg);
};

var embederWindowChannel = null;

var notifyEmbedder = function(msg_array) {
  embederWindowChannel.postMessage(JSON.stringify(msg_array), '*');
};

var runSpeechRecognitionAPI = function() {
  LOG('runSpeechRecognitionAPI');
  var r = new webkitSpeechRecognition();
  var succeeded = false;
  r.onstart = function() {
    succeeded = true;
    LOG('r.onstart');
    notifyEmbedder(['recognition', 'onstart', '']);
    r.abort();
  };
  r.onerror = function() {
    LOG('r.onerror');
    if (succeeded) {
      return;
    }
    notifyEmbedder(['recognition', 'onerror', '']);
  };
  r.onresult = function(e) {
    LOG('r.onresult');
    if (!e.results || !e.results.length) {
      notifyEmbedder(['recognition', 'unknown', '']);
      return;
    }
    var transcript = e.results[0][0].transcript;
    notifyEmbedder(['recognition', 'onresult', transcript]);
  };
  r.start();
};

var onPostMessageReceived = function(e) {
  embederWindowChannel = e.source;
  var data = JSON.parse(e.data);
  if (data[0] == 'create-channel') {
    notifyEmbedder(['channel-created']);
  } else if (data[0] == 'runSpeechRecognitionAPI') {
    runSpeechRecognitionAPI();
  }
};

window.addEventListener('message', onPostMessageReceived);
LOG('guest.js load end');
