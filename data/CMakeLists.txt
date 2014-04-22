install ( FILES main.cfg DESTINATION share/glportal/data )

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION share/glportal
                    FILES_MATCHING PATTERN "*.png")

install ( FILES shaders/ppl.vert DESTINATION share/glportal/data/shaders )
install ( FILES shaders/nmap.vert DESTINATION share/glportal/data/shaders )

install ( FILES audio/music/track1.ogg DESTINATION share/glportal/data/audio/music )
