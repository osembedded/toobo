Roadmap for toobo Version 1.0 Laika

# Proposed Features #
Laika is going to include preliminary support for the following features -
  * Monochrome graphic LCD display
  * MP3 audio decoding
  * Linux kernel 2.6
  * USB wireless connectivity

# Timeline #

  * Running Kernel 2.6 on the target - 06-22-08
    * 2.6 kernel provided by embedded arm runs on the target. Debian Sarge 3.1 root file system also loads via nfs.
    * Came across the embedded debian website (emdebian - http://www.emdebian.org/). Need to investigate it.
  * Porting the graphic LCD driver to 2.6 - 06-29-08
    * GLCD Module has been ported for the 2.6 Kernel. Can insert and remove module on the fly.
    * Test application needs to be ported before module can be tested.
    * GLCD Module has not be ported but has been compiled with the new 2.6 Kernel Make file system and works fine with the test application. - 07-27-08

_Due to various time constraints, the following time line has been updated. 07-27-08_
  * Preliminary MP3 decoding capability - 08-03-08
  * Working MP3 decoding capability - 08-17-08
  * Investigate USB wireless device - 08-17-08
  * Support for USB wireless device - 08-24-08
  * Release of toobo Version 1.0 Laika 08-31-08