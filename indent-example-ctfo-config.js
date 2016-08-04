#!/usr/bin/env node

// JavaScript is the cleanest way to format JSON w/o reordering the fields.
// Note: It would mess up int64 values that don't fit 53 bits. Oh well. There are none in our data. -- D.K.

const fs = require('fs');
const fn = 'ctfo_config.json_example';

fs.writeFileSync(fn, JSON.stringify(JSON.parse(fs.readFileSync(fn).toString()), null, 2));
