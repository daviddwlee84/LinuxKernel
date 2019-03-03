# Build Basic Kernel

## [Hello World](HelloWorld)

* MACROs
  * `__init` & `__exit`
  * `module_init()` & `module_exit()`
* Basic Makefile
* Install and Remove

## [Passing Command Line Arguments](ParameterPassing)

* MACRO
  * `module_param()`
* Install and pass parameter from command line
* Licensing and module description
  * `MODULE_LICENSE()`
  * `MODULE_DESCRIPTION()`
  * `MODULE_AUTHOR()`

## Export Symbol

A module wants to export a symbol (i.e. *function* or *global variable*)

* MACROs
  * `EXPORT_SYMBOL()`
  * `EXPORT_SYMBOL_GPL()` (only the kernrl who also uses GPL license can use the exported symbol)

> A symbol must be exported at the global part of the code.
