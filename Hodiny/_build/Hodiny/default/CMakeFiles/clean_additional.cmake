# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.cmf"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.hex"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.hxl"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.mum"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.o"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.sdb"
  "/home/nontoxiccat/Desktop/KiCad projects/Rocnikovy_Projekt_S3E/Hodiny/out/Hodiny/default.sym"
  )
endif()
