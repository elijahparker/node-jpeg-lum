About
========
Calculates the luminance value and histogram of JPEG images.  This is based on Bill Yeh's excellent node-pixelr, adapted for getting luminance and histogram data instead of raw pixels.

Example
==========
```
var luminance = require('luminance');

/** luminance.read(filename, callback)
 * Filename points to a JPEG image
 * The callback takes an object with properties 'histArray' (an array), 'luminance', width', and 'height'.
 */
luminance.read("image.jpeg", function(err, res) {
  console.log(res);
});


```

Documentation
===============
JPEG parsing uses `libjpeg`


Installation
===============
```
$ npm install luminance
```

License
=========

(The MIT License)

Copyright (c) 2013 Bill Yeh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
