if(NOT TARGET fcf::fcfTest)
  add_library(fcf::fcfTest INTERFACE IMPORTED)

  get_filename_component(_local_path "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
  get_filename_component(_vcpkg_path "${CMAKE_CURRENT_LIST_DIR}/../../include" ABSOLUTE)

  set(_all_paths "${_local_path}" "${_vcpkg_path}")
  set(_valid_paths "")

  foreach(_path IN LISTS _all_paths)
    if(IS_DIRECTORY "${_path}")
      list(APPEND _valid_paths "${_path}")
    endif()
  endforeach()

  set_target_properties(fcf::fcfTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_valid_paths}"
  )

  unset(_local_path)
  unset(_vcpkg_path)
  unset(_all_paths)
  unset(_valid_paths)
endif()

