# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Hodiny_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(Hodiny_default_default_XC8_FILE_TYPE_assemble)
add_library(Hodiny_default_default_XC8_assemble OBJECT ${Hodiny_default_default_XC8_FILE_TYPE_assemble})
    Hodiny_default_default_XC8_assemble_rule(Hodiny_default_default_XC8_assemble)
    list(APPEND Hodiny_default_library_list "$<TARGET_OBJECTS:Hodiny_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(Hodiny_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(Hodiny_default_default_XC8_assemblePreprocess OBJECT ${Hodiny_default_default_XC8_FILE_TYPE_assemblePreprocess})
    Hodiny_default_default_XC8_assemblePreprocess_rule(Hodiny_default_default_XC8_assemblePreprocess)
    list(APPEND Hodiny_default_library_list "$<TARGET_OBJECTS:Hodiny_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(Hodiny_default_default_XC8_FILE_TYPE_compile)
add_library(Hodiny_default_default_XC8_compile OBJECT ${Hodiny_default_default_XC8_FILE_TYPE_compile})
    Hodiny_default_default_XC8_compile_rule(Hodiny_default_default_XC8_compile)
    list(APPEND Hodiny_default_library_list "$<TARGET_OBJECTS:Hodiny_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(Hodiny_default_image_l6s2x_of ${Hodiny_default_library_list})

set_target_properties(Hodiny_default_image_l6s2x_of PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Hodiny_default_output_dir})
set_target_properties(Hodiny_default_image_l6s2x_of PROPERTIES OUTPUT_NAME "default")
set_target_properties(Hodiny_default_image_l6s2x_of PROPERTIES SUFFIX ".elf")

target_link_libraries(Hodiny_default_image_l6s2x_of PRIVATE ${Hodiny_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
Hodiny_default_link_rule(Hodiny_default_image_l6s2x_of)



