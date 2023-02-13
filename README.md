# Caravel User Project

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![UPRJ_CI](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml) [![Caravel Build](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml)

| :exclamation: Important Note            |
|-----------------------------------------|

## Please fill in your project documentation in this README.md file 

Refer to [README](docs/source/index.rst#section-quickstart) for a quickstart of how to use caravel_user_project

Refer to [README](docs/source/index.rst) for this sample project documentation. 

## Steps:
1. Integrate modules into the user_project_wrapper
- Change the environment variables `VERILOG_FILES_BLACKBOX`, `EXTRA_LEFS` and `EXTRA_GDS_FILES `in `openlane/user_project_wrapper/config.json`
- Copy the macro.module files from `Final` folder `vsdmemsoc.v` to `<caravel folder>/verilog/rtl ;gds ; lef ;` folder
- Change the `user_project_wrwpper.v` to include the new design
- Update the power supplies connections in the design according to user_project_wrwpper.v` 
- Update the include file `<caravel folder>/verilog/rtl/user_project_.v`
- add tests - adapt from other tests

```
  ── vsdmemsoc
│   ├── Makefile
│   ├── vsdmemsoc.c
│   └── vsdmemsoc_tb.v
``` 
- check the test/trace
