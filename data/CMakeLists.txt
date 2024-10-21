# Copyright © 2024 Jason Stredwick

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL YES)

project(
  jcu_download_data
  VERSION 0.1
  DESCRIPTION "Download data files required for code_gen."
  LANGUAGES CXX)

include(ExternalProject)

set(jcu_download_data_VERSION "latest")
string(CONCAT jcu_download_data_BaseURL "https://www.unicode.org/Public/UCD/" "${jcu_download_data_VERSION}" "/ucd/")


function(jcu_download_data_DoDownload file_path download_dir)
  string(CONCAT url_str "${jcu_download_data_BaseURL}" "${file_path}")
  string(CONCAT proj_name_raw "jcu_download_data_" "${file_path}")
  string(REGEX REPLACE "[/.:]" "_" proj_name "${proj_name_raw}")
  ExternalProject_Add(
    ${proj_name}
    URL "${url_str}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    DOWNLOAD_DIR "${download_dir}"
    DOWNLOAD_NO_EXTRACT TRUE
    DOWNLOAD_NO_PROGRESS 1
    INACTIVITY_TIMEOUT 60
    INSTALL_COMMAND ""
    TIMEOUT 60
  )
endfunction()

set(
  jcu_download_data_urls
  "BidiBrackets.txt"
  "BidiCharacterTest.txt"
  "BidiMirroring.txt"
  "BidiTest.txt"
  "extracted/DerivedBidiClass.txt"
  "extracted/DerivedGeneralCategory.txt"
  "PropertyValueAliases.txt"
  "Scripts.txt"
  "UnicodeData.txt"
  "VerticalOrientation.txt"
)

foreach(item IN LISTS jcu_download_data_urls)
  jcu_download_data_DoDownload("${item}" "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach()
