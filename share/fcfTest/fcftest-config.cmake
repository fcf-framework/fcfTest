if(NOT TARGET fcf::fcfTest)
  add_library(fcf::fcfTest INTERFACE IMPORTED)

  get_filename_component(_current_path "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
  get_filename_component(_local_path "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
  get_filename_component(_vcpkg_path "${CMAKE_CURRENT_LIST_DIR}/../../include" ABSOLUTE)

  set(_all_paths "${_local_path}" "${_vcpkg_path}")
  set(_valid_paths "")

  foreach(_path IN LISTS _all_paths)
    if(IS_DIRECTORY "${_path}/fcfTest" )
      list(APPEND _valid_paths "${_path}")
    endif()
  endforeach()

  if(NOT _valid_paths)
    set(_proxy_root "${CMAKE_CURRENT_BINARY_DIR}/fcf_include_proxy")
    set(_target_dir "${_proxy_root}/fcfTest")
    if(NOT IS_DIRECTORY "${_target_dir}")
      file(MAKE_DIRECTORY "${_target_dir}")
    endif()

    file(COPY "${_current_path}/"
         DESTINATION "${_target_dir}")

    list(APPEND _valid_paths "${_proxy_root}")
  endif()

  set_target_properties(fcf::fcfTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_valid_paths}"
  )

  unset(_current_path)
  unset(_local_path)
  unset(_vcpkg_path)
  unset(_all_paths)
  unset(_valid_paths)
  unset(_proxy_root)
  unset(_target_dir)
  
endif()
