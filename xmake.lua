-- set xmake minimal version
set_xmakever("2.3.8")

-- set repository description
set_description("The official package repository of xmake")


-- dependencies
add_requires("libsdl")

-- target
target("debug")
    set_kind("binary")
    add_files("src/**.c")
    add_headerfiles("src/**.h")
    add_packages("libsdl")
