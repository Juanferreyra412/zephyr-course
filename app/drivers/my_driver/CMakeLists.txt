if(CONFIG_MY_DRIVER)
    target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/my_driver.c)
endif()
