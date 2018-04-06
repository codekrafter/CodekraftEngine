cmake_minimum_required(VERSION 3.5)

add_subdirectory(engine)

add_subdirectory(testgame)

<<<<<<< HEAD
add_custom_target(build DEPENDS CKEngine TestGame)
=======
add_custom_target(build DEPENDS TestGame CKEngine)
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba
