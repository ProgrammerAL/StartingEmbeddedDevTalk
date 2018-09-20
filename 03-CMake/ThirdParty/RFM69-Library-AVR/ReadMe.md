
Code in this repository is a fork from https://github.com/nayem-cosmic/RFM69-Library-AVR taken August 15th, 2018

Original License: Unsure. Some code is GPL3.0, some is GPL2.
With changes made, this code has been relicensed under LGPLv2.1 as described at the GNU FAQ https://www.gnu.org/licenses/gpl-faq.html#compat-matrix-footnote-7

List of Changes made:

- Changes to the lib so it can be used by C++. Added a handful of 'extern "C"' blocks
- Removed millis.c file and now pass in a function pointer to be called by this library to get the milliseconds

