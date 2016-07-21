#!/usr/bin/env node

var jpeg = require('./build/Release/jpeg-lum')
  , fs   = require('fs');

exports.read = jpeg.read;