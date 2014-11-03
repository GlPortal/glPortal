set ( SHARE_INSTALL_DIR share/glportal )

install ( FILES main.cfg DESTINATION ${SHARE_INSTALL_DIR}/data )

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.png")

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.tga")
install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.obj")         
 install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.mtl") 
                    
install ( FILES maps/maplist DESTINATION ${SHARE_INSTALL_DIR}/data/maps )
install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${SHARE_INSTALL_DIR}
                    FILES_MATCHING PATTERN "*.xml")

install ( FILES shaders/diffuse.vert DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )
install ( FILES shaders/diffuse.frag DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )
install ( FILES shaders/unshaded.frag DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )
install ( FILES shaders/text.frag DESTINATION ${SHARE_INSTALL_DIR}/data/shaders )

install ( FILES audio/music/track1.ogg DESTINATION ${SHARE_INSTALL_DIR}/data/audio/music )
