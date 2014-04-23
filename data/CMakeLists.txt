set ( SHARE_INSTALL_DIR share/glportal )

install ( FILES main.cfg DESTINATION ${SHARE_INSTALL_DIR}/data )

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.png")

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.tga")
      
install ( FILES shaders/ppl.vert DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )
install ( FILES shaders/nmap.vert DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )

install ( FILES audio/music/track1.ogg DESTINATION ${SHARE_INSTALL_DIR}/data/audio/music )
