nvram-faker
===========

When using emulation to run an application found in an embedded Linux firmware, such as a wireless router's web server, one of the main problems encountered is the application attempting to source NVRAM for configuration parameters.  A common library, `libnvram.so`, is often used in embedded Linux to abstract access to NVRAM.  This provides `nvram_get()` and `nvram_set()` functions to get and set configuration parameters.  The calls to `nvram_get()` will fail, since the emulated environment has no NVRAM.  Without configuration parameters the target application will likely fail to run.

`nvram-faker` is a simple library to intercept calls to libnvram using `LD_PRELOAD`.  By providing sane values in an INI-style NVRAM configuration file you can answer queries to NVRAM, enabling the application to start up and run.


BUILDING
--------------------------

There should be no external dependencies.  I have tested building in Ubuntu Linux.

You can build the shared library, `libnvram-faker.so`, simply by typing `make`.  However, you'll almost certainly want to cross-compile for your target architecture.  For convenience, I've provided shell scripts for building on big and little endian MIPS, since these are the most common architectures I work with.  The shell scripts set up your toolchain variables such as $CC to point to your cross-compilation tools, if they're in your $PATH.  If you need to cross compile with other architectures, such as ARM, this shouldn't be too hard to set up.


USING
-------------------------

Put `libnvram-faker.so` somewhere in your firmware's extracted filesystem.  Edit `nvram.ini` and put it in the root of the firmware's filesystem.  If you want to put the configuration file somewhere else, define `INI_FILE_PATH` at compile time however you like[1].  Set the `LD_PRELOAD` environment variable to load the library.

Every nvram query and the answer returned will be printed to the console's standard output.  If the answer is not found, NULL is returned by `nvram_get` and the query is printed to the console in red, so it will easy to notice.

The library currently does not support calls to `nvram_set()`.


ACKNOWLEDGMENTS
-------------------------

I shamelessly aped this idea from Craig Heffner <cheffner@tacnetsol.com>, ([@devttyS0](https://twitter.com/devttyS0) on Twitter).  I improved on it by adding support for INI configuration and colorized console output.

INI support is provided by this excellent [project](http://code.google.com/p/inih/), which I have included this source tree.

---

[1] You can export a `CFLAGS` environment variable on the command line to do this.