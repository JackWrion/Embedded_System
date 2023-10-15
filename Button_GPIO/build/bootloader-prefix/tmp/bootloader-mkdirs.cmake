# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.0.1/components/bootloader/subproject"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/tmp"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/src"
  "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Espressif/workspace/Button_GPIO/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
