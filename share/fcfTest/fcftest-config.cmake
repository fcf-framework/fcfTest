if(NOT TARGET fcf::fcfTest)
  add_library(fcf::fcfTest INTERFACE IMPORTED GLOBAL)

  get_filename_component(_local_path "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
  get_filename_component(_vcpkg_path "${CMAKE_CURRENT_LIST_DIR}/../../include" ABSOLUTE)

  set(_all_paths "${_local_path}" "${_vcpkg_path}")
  set(_valid_paths "")

  foreach(_path IN LISTS _all_paths)
    if(IS_DIRECTORY "${_path}")
      get_filename_component(_dir_name "${_path}" NAME)

      if(EXISTS "${_path}/test.hpp" AND NOT "${_dir_name}" STREQUAL "fcfTest")
        set(_proxy_root "${CMAKE_CURRENT_BINARY_DIR}/fcf_include_proxy")
        set(_target_dir "${_proxy_root}/fcfTest")

        if(NOT IS_DIRECTORY "${_target_dir}")
          file(MAKE_DIRECTORY "${_target_dir}")
        endif()

        if(NOT EXISTS "${_target_dir}/test.hpp")
          file(WRITE "${_target_dir}/test.hpp" "#include \"${_path}/test.hpp\"\n")
        endif()

        list(APPEND _valid_paths "${_proxy_root}")
      else()
        get_filename_component(_parent_path "${_path}/.." ABSOLUTE)
        list(APPEND _valid_paths "${_parent_path}")
      endif()
    endif()
  endforeach()

  set_target_properties(fcf::fcfTest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_valid_paths}"
  )

  unset(_local_path)
  unset(_vcpkg_path)
  unset(_all_paths)
  unset(_valid_paths)
  unset(_dir_name)
  unset(_proxy_root)
  unset(_target_dir)
  unset(_parent_path)
endif()

