/*
fmod for Javascript, will work with any ECMA-262 implementation.
If you need a precision higher than 8, please use another implementation of fmod.
1.05 % 0.05
  => 0.04999999999999999
Math.fmod(1.05, 0.05)
  => 0
Math.fmod(1.05, 0.019)
  => 0.005
Math.fmod(1.00000012, 0.00000005)
  => 0.00000002
fmod will most likely fail, if (Math.log(a)/Math.log(10) - Math.log(b)/Math.log(10)) > 8
Try to keep the difference in order of magnitudes as small as possible!:
Math.fmod(1000000.40, 0.07)
  => 0.07
Math.fmod(10000000.40, 0.07)
  => 0.039999999
*/

/* TODO: fix for  a=4.55  b=0.05  !!*/

Math.fmod = function (a,b) { return Number((a - (Math.floor(a / b) * b)).toPrecision(8)); };
