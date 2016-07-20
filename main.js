#!/usr/bin/env node

var jpeg = require('./build/Release/luminance')
  , fs   = require('fs');

exports.read = jpeg.read;